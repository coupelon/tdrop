/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "tddaemon.h"

tdParam *TdDaemon::tdp = NULL;
users *TdDaemon::clients = NULL;
UIDSession *TdDaemon::searches = NULL;

string TdDaemon::save_config(string s) {
	xmlFile xf;
	getHttp gh;
	string path = selectFile::getHomeDirectory();
	gh.setParam(tdp);
	if (xf.openMemory(s)) {
		//Write the data to the config file.
		ofstream config;
		config.open(selectFile::getHomeConfigFile().c_str(), ios::out | ios::trunc | ios::binary);
		if (config.is_open()) {
			config << "<?xml version=\"1.0\"?>" << endl << "<config>" << endl;
			nodeDoc ndCateg(&xf,"category");
			while(ndCateg.isValid()) {
				config << "\t<category name=\"" << ndCateg.getAttributeValueByName("name");
				config << "\">" << endl;
				nodeDoc ndEngine(&xf,"engine",ndCateg);
				while(ndEngine.isValid()) {
					string url = ndEngine.getAttributeValueByName("url");
					string file = ndEngine.getAttributeValueByName("path");
					if (url != "") {
						file =  "xml/" + selectFile::getFilename(url);
						address ad;
						ad.url = url;
						if (gh.getFile(ad,path + file)) {
							//The url should be of the form http://*/xml/*.xml
							//TODO: retrieve the icon.
							string icon = "icons/" +
														selectFile::getBasename(selectFile::getFilename(url)) +
														".png";
							ad.url = url.substr(0,url.find("/xml")) + "/" + icon;
							gh.getFile(ad,path + icon);
						}
					}
					config << "\t\t<engine path=\"" << file
								 << "\" version=\"" << ndEngine.getAttributeValueByName("version")
								 << "\" />" << endl;
					ndEngine.next();
				}
				config << "\t</category>" << endl;
				ndCateg.next();
			}
			config << "</config>" << endl;
			config.close();
		} else {
			LOG4CXX_DEBUG(tdParam::logger, "Couldn't load configuration file.");
		}
		tdp->commit();
	}
	return DEFAULT_HEADER_NL;
}

string TdDaemon::createJSON(bool final, metaRank *mr) {
	string pre;
	if (final) {
		pre = "{\"final\":\"true\",";
	} else {
		pre = "{\"final\":\"false\",";
	}
	map<string,int> *map_engines = mr->getEngineResultsNumber();
	if (map_engines) {
		pre += "\"engines\":[";
		char number[11];
		for(map<string,int>::iterator me_it = map_engines->begin();
			  me_it != map_engines->end(); ++me_it) {
			  sprintf(number,"%i",me_it->second);
			  pre += (me_it == map_engines->begin())?"":",";
			  pre += "{\"name\":\"";
				pre += me_it->first;
				pre += "\",\"cpt\":";
			  pre += number;
			  pre += "}";
	  }
	  pre += "],";
	}
	
	pre += "\"results\":[{";
	
  string text = string("\"num\":{num},") +
  							  "\"engines\":\"{engines}\"," +
  							  "\"title\":\"{title}\"," +
  							  "\"url\":\"{url}\"," +
  							  "\"img\":\"{img}\"," +
  							  "\"abstract\":\"{abstract}\"";
  string post = "}]}";
  return mr->getString(text,"/","},{",pre,post,true);
}

string TdDaemon::get_authenticate_user(struct shttpd_arg *arg, string user,string pass) {
	string id = clients->authenticateUser(user, pass, shttpd_get_env(arg, "REMOTE_ADDR"));
	string out = DEFAULT_HEADER;
	if (id != "") {
		out += "Set-Cookie: ID=" + id + "\r\n"; 
	}
	return out + "\r\n";
}

string TdDaemon::newQuery(struct shttpd_arg *arg, string query,string engines,string limit) {
	list<string> lse;
  engineResults *er = new engineResults();
  er->setQuery(query);
  er->setLimit(atol(limit.c_str()));
  size_t pos;
  while((pos = engines.find(","))!=string::npos) {
  	er->addEngine(engines.substr(0,pos));
  	engines = engines.substr(pos+1);
  }
  er->addEngine(engines);
  string userID = shttpd_get_header(arg, "Cookie");
  string newID = searches->addSearch(userID,new metaRank(er,tdp),clients->getUsername(userID));
  searches->getSearch(userID,newID,clients->getUsername(userID))->startParsing();
  string output = DEFAULT_HEADER;
  output += "Set-TDSession: query=" + newID + ";\r\n\r\n";
  return output + createJSON(searches->getSearch(userID,newID,clients->getUsername(userID))->waitForNewResults(),
   					 searches->getSearch(userID,newID,clients->getUsername(userID)));
}

string TdDaemon::get_next_results(struct shttpd_arg *arg) {
	const char *cookie_string = shttpd_get_header(arg, "TDSession");
	string userID = shttpd_get_header(arg, "Cookie");
	if(cookie_string) {
		regExp r("query=([a-zA-Z0-9]*)");
		string cstring = cookie_string;
	  	r.newPage(cstring);
		if (!r.endOfMatch()) {
			string newID(r.getMatch(1));
			if (searches->getSearch(userID,newID,clients->getUsername(userID)) != NULL) {
			  return DEFAULT_HEADER_NL + createJSON(searches->getSearch(userID,newID,clients->getUsername(userID))->waitForNewResults(),
	   					 searches->getSearch(userID,newID,clients->getUsername(userID)));
			}
    	}
	}
	LOG4CXX_DEBUG(tdParam::logger, "An error occurred while retrieving next results.");
  return DEFAULT_HEADER_NL;
}

string TdDaemon::export_csv(struct shttpd_arg *arg) {
	string userID = shttpd_get_header(arg, "Cookie");
	regExp r("query=([a-zA-Z0-9]*)");
	string cstring = shttpd_get_env(arg, "QUERY_STRING");
  	r.newPage(cstring);
	if (!r.endOfMatch()) {
		string newID(r.getMatch(1));
		metaRank *mr = searches->getSearch(userID,newID,clients->getUsername(userID));
		if (mr != NULL) {
		  return CSV_HEADER_NL + openSave::csvExport(mr);
		}
	}
	LOG4CXX_DEBUG(tdParam::logger, "An error occurred while trying to export.");
  return DEFAULT_HEADER_NL;
}

/*
 * This callback function is attached to the "/" and "/abc.html" URIs,
 * thus is acting as "index.html" file. It shows a bunch of links
 * to other URIs, and allows to change the value of program's
 * internal variable. The pointer to that variable is passed to the
 * callback function as arg->user_data.
 */
void TdDaemon::show_index(struct shttpd_arg *arg) {
	shttpd_printf(arg, "%s", "HTTP/1.1 301 Moved Permanently\r\n");
	shttpd_printf(arg, "%s", "Location: /WebInterface/WebInterface.html\r\n");
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

/*
 * This callback function is used to get the JSON of the engines tree
 */
string TdDaemon::show_tree() {
	xmlFile xf;
	string config_path;
	string output = DEFAULT_HEADER_NL;
	
	if (selectFile::find(CONFIG_FILE,config_path) && xf.openFile(config_path)) {
		nodeDoc ndCateg(&xf,"category");
		output += "{\"categories\":[";
		while(ndCateg.isValid()) {
			//Prints the category name
			output += "{\"name\":\"";
			output += ndCateg.getAttributeValueByName("name");
			output += "\",\"icon\":\"folder.png\",\"engines\":[";
			nodeDoc ndEngine(&xf,"engine",ndCateg);
			while(ndEngine.isValid()) {
				output += "{\"name\":\"";
				string filepath = ndEngine.getAttributeValueByName("path");
				output += filepath;
				string filename = selectFile::getFilename(filepath);
				//Only keep the filename without extension
				filename = selectFile::getBasename(filename);

				output += "\",\"icon\":\"";
				output += "/engines_icons/" + filename + ".png";
				output += "\",\"title\":\"";
				
				xmlEngine xe;
				if (xe.openEngine(filepath)) {
					output += xe.getName();
				} else {
					output += filename;
				}

				output += "\"}";
				ndEngine.next();
				if (ndEngine.isValid()) output += ",";
			}
			output += "]}";
			ndCateg.next();
			if (ndCateg.isValid()) output += ",";
		}
		output += "]}";
	} else {
		output += "Teardrop configuration file could not be found.";
	}
	return output;
}

string TdDaemon::show_available_engines() {
	xmlFile xf;
	string config_path;
	string output = DEFAULT_HEADER_NL;
  	output += "{\"engines\":[";
	if (selectFile::find(CONFIG_FILE,config_path) && xf.openFile(config_path)) {
		nodeDoc ndCateg(&xf,"category");
		while(ndCateg.isValid()) {
			nodeDoc ndEngine(&xf,"engine",ndCateg);
			string engines;
			while(ndEngine.isValid()) {
				engines += "{\"select\":\"true\",\"categ\":\"" + ndCateg.getAttributeValueByName("name") + "\",";
				string filepath = ndEngine.getAttributeValueByName("path");
				engines += "\"file\":\"" + filepath + "\",\"name\":\"";

				xmlEngine xe;
				if (xe.openEngine(filepath)) {
					engines += xe.getName();
				} else {
					engines += selectFile::getFilename(filepath);;
				}				
				engines += "\",";
				string version = ndEngine.getAttributeValueByName("version");
				engines += "\"version\":\"" + version + "\"}";

				ndEngine.next();
				if (ndEngine.isValid()) engines += ",";
			}
			ndCateg.next();
			output += engines;
			if (engines != "") output += ",";
		}
	}
	
	getHttp gh;
	gh.setParam(tdp);
	address ad;
  ad.url = UPDATE_URL;
  string page = gh.getPage(ad);
  
	if (page != "" && xf.openMemory(page)) {
		nodeDoc ndCateg(&xf,"category");
		while(ndCateg.isValid()) {
			nodeDoc ndEngine(&xf,"engine",ndCateg);
			while(ndEngine.isValid()) {
				output += "{\"select\":\"false\",\"categ\":\"" + ndCateg.getAttributeValueByName("name") + "\",";
				string filepath = ndEngine.getAttributeValueByName("url");
				output += "\"file\":\"" + filepath + "\",\"name\":\"";

				output += ndEngine.getAttributeValueByName("name");	
				
				output += "\",";
				string version = ndEngine.getAttributeValueByName("version");
				output += "\"version\":\"" + version + "\"}";

				ndEngine.next();
				if (ndEngine.isValid()) output += ",";
			}
			ndCateg.next();
			if (ndCateg.isValid()) output += ",";
		}
	}
	output+="]}";
	return output;
}

/*
 * This callback function is used to show how to handle 404 error
 */
void TdDaemon::show_404(struct shttpd_arg *arg) {
	shttpd_printf(arg, "%s", "HTTP/1.1 404 OK\r\n");
	shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");
	shttpd_printf(arg, "%s", "404 file not found! ");
	LOG4CXX_INFO(tdParam::logger, "Error 404: " << shttpd_get_env(arg, "REQUEST_URI"));
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

void TdDaemon::query_process(struct shttpd_arg *arg) {
	const char	*s;//, *path = "uploaded.txt";
	struct state {
		unsigned long int cl;		/* Content-Length	*/
		size_t	nread;		/* Number of bytes read	*/
		string	buffer;
		size_t count;
	} *state;
	string uri = shttpd_get_env(arg, "REQUEST_URI");

	/* If the connection was broken prematurely, cleanup */
	if (arg->flags & SHTTPD_CONNECTION_ERROR && arg->state) {
		delete (struct state *)arg->state;
	} else if ((uri == QUERY_POST ||
						  uri == SAVE_ENGINES ||
              uri == AUTHENTICATE_USER) &&
             (s = shttpd_get_header(arg, "Content-Length")) == NULL) {
		shttpd_printf(arg, "HTTP/1.0 411 Length Required\n\n");
		LOG4CXX_INFO(tdParam::logger, "Error 411: Content-Length not specified.");
		arg->flags |= SHTTPD_END_OF_OUTPUT;
	} else if (arg->state == NULL && !clients->isValid(shttpd_get_header(arg, "Cookie"),shttpd_get_env(arg, "REMOTE_ADDR")) && uri != AUTHENTICATE_USER) {
	    if (!clients->authenticationRequired()) {
	    	shttpd_printf(arg, "%s", "HTTP/1.1 301 Moved Permanently\r\n");
	        string id = UIDSession::getID();
		    clients->addUser(id,string(shttpd_get_env(arg, "REMOTE_ADDR")), "default");
		    shttpd_printf(arg, "%s%s%s", "Set-Cookie: ID=", id.c_str(),"\r\n");
	        shttpd_printf(arg, "%s%s%s", "Location: ",shttpd_get_env(arg, "REQUEST_URI"),"\r\n");
		    LOG4CXX_INFO(tdParam::logger, "Error 301: Reload the page with the correct cookie informations, for default user.");
		    arg->flags |= SHTTPD_END_OF_OUTPUT;
	    } else {
		    shttpd_printf(arg, "HTTP/1.0 403 Authentication Required\n\n");
		    LOG4CXX_INFO(tdParam::logger, "Error 403: User not authenticated.");
		    arg->flags |= SHTTPD_END_OF_OUTPUT;
		}
  } else if (arg->state == NULL) {
		/* New request. Allocate a state structure */
		arg->state = state = new (struct state);
		if (uri == QUERY_POST ||
        uri == SAVE_ENGINES ||
        uri == AUTHENTICATE_USER)
			state->cl = strtoul(s, NULL, 10);
		else
			state->cl = 0;
		state->buffer = "";
		state->nread = 0;
		state->count = 0;
	} else {
		state = (struct state *) arg->state;
		/*
		 * Write the POST data to a file. We do not do any URL
		 * decoding here. File will contain form-urlencoded stuff.
		 */
		state->buffer.append(arg->in.buf, arg->in.len);
		state->nread += arg->in.len;

		/* Tell SHTTPD we have processed all data */
		arg->in.num_bytes = arg->in.len;

		/* Data stream finished? Close the file, and free the state */
		if (state->nread >= state->cl) {
			if (state->count == 0) {
				if (uri == NEXT_RESULTS) {
					state->buffer = get_next_results(arg);
				} else if (uri == QUERY_POST) {
					regExp r("query=(.+);engines=([^;]+);limit=([1-9][0-9]{1,3})");
					r.newPage(state->buffer);
					if (!r.endOfMatch()) {
				    state->buffer = newQuery(arg,r.getMatch(1),r.getMatch(2),r.getMatch(3));
				    } else {
				        state->buffer = "{\"results\":[{}]}";
					}
				} else if (uri == AUTHENTICATE_USER) {
					regExp r("username=([^\\&]+)\\&password=([^\\&]+)");
					r.newPage(state->buffer);
					if (!r.endOfMatch()) {
						state->buffer = get_authenticate_user(arg,r.getMatch(1),r.getMatch(2));
					}
				} else if (uri == REQUEST_TREE) {
					state->buffer = show_tree();
				} else if (uri == AVAILABLE_ENGINES) {
					state->buffer = show_available_engines();
				} else if (uri == SAVE_ENGINES) {
					state->buffer = save_config(state->buffer);
				} else if (uri == EXPORT_CSV) {
					state->buffer = export_csv(arg);
				} else if (uri == GET_USER_HISTORY) {
				  state->buffer = clients->getUserHistory(shttpd_get_header(arg, "Cookie"),shttpd_get_env(arg, "REMOTE_ADDR"));
				}
			}
			while (arg->out.num_bytes < arg->out.len && state->count < state->buffer.length()) {
				arg->out.buf[arg->out.num_bytes] = state->buffer[state->count];
				arg->out.num_bytes++;
				state->count++;
			}
			if (state->count >= state->buffer.length()) {
				arg->flags |= SHTTPD_END_OF_OUTPUT;
				delete (struct state *) arg->state;
			}
	  }
	}
}

void TdDaemon::show_file(struct shttpd_arg *arg,string filename) {
	FILE *fp = fopen(filename.c_str(),"rb");
  if (fp == NULL) {
  	show_404(arg);
  } else {
	int state = (int) arg->state;

	if (state == 0) {
		shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n\r\n");
	} else {
		fseek(fp,state,SEEK_SET);
	}
	char b;
	while (arg->out.num_bytes < arg->out.len && !feof(fp)) {
		fread(&b,1,1,fp);
		arg->out.buf[arg->out.num_bytes] = b;
		arg->out.num_bytes++;
		state++;
	}
	if (feof(fp)) {
		arg->flags |= SHTTPD_END_OF_OUTPUT;
	}
	arg->state = (void *) state;
	fclose(fp);
  }
}

void TdDaemon::show_engines_icons(struct shttpd_arg *arg) {
	string filename = string(WEBINTERFACE_ENGINES_ICONS) + string(shttpd_get_env(arg, "REQUEST_URI")).erase(0,WEBINTERFACE_ENGINES_ICONS_SIZE);
	FILE *fp = fopen(filename.c_str(),"rb");
	if (fp == NULL) filename = string (WORKSPACE) + string(WEBINTERFACE_PATH) + "imgs/default_engine.png";
	else fclose(fp);
	show_file(arg, filename);
}


void TdDaemon::show_wi(struct shttpd_arg *arg) {
	string filename = WORKSPACE + string(WEBINTERFACE_PATH) + string(shttpd_get_env(arg, "REQUEST_URI")).erase(0,WEBINTERFACE_ROOT_SIZE);
	show_file(arg, filename);
}

void TdDaemon::launchDaemon(tdParam *t) {
	struct shttpd_ctx	*ctx;
	
	tdp = t;
	
	LOG4CXX_INFO(tdParam::logger, "Teardrop started.");
	clients = new users();
	searches = new UIDSession(tdp);

	signal(SIGPIPE, SIG_IGN);

	/*
	 * Initialize SHTTPD context.
	 * Aget_next_resultsttach folder c:\ to the URL /my_c  (for windows), and
	 * /etc/ to URL /my_etc (for UNIX). These are Apache-like aliases.
	 * Set WWW root to current directory.
	 * Start listening on ports 8080 and 8081
	 */
	ctx = shttpd_init();
	shttpd_set_option(ctx, "ssl_cert", string(selectFile::getHomeDirectory() +"teardrop.pem").c_str());
	shttpd_set_option(ctx, "ports", "8080,8081s");

	shttpd_register_uri(ctx, "/", &show_index, NULL);
	shttpd_register_uri(ctx, REQUEST_TREE, &query_process, NULL);
	shttpd_register_uri(ctx, QUERY_POST, &query_process, NULL);
	shttpd_register_uri(ctx, NEXT_RESULTS, &query_process, NULL);
	shttpd_register_uri(ctx, WEBINTERFACE_ENGINES_ICONS_URI, &show_engines_icons, NULL);
	shttpd_register_uri(ctx, WEBINTERFACE_URI, &show_wi, NULL);
	shttpd_register_uri(ctx, AVAILABLE_ENGINES, &query_process, NULL);
	shttpd_register_uri(ctx, SAVE_ENGINES, &query_process, NULL);
	shttpd_register_uri(ctx, AUTHENTICATE_USER, &query_process, NULL);
	shttpd_register_uri(ctx, EXPORT_CSV, &query_process, NULL);
	shttpd_register_uri(ctx, GET_USER_HISTORY, &query_process, NULL);

	shttpd_handle_error(ctx, 404, show_404, NULL);

	/* Serve connections infinitely until someone kills us */
	for (;;)
		shttpd_poll(ctx, 1000);

	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);
}
