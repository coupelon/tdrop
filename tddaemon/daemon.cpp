/*
 * This file is an example of how to embed web-server functionality
 * into existing application.
 * Compilation line:
 * cc example.c shttpd.c -DEMBEDDED
 */


#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>

#include <fstream>

#include "shttpd.h"

#include "nodeDoc.h"
#include "xmlEngine.h"
#include "selectFile.h"
#include "regExp.h"
#include "metaRank.h"

#define WEBINTERFACE_URI "/WebInterface/*"
#define WEBINTERFACE_PATH "/home/olivier/workspace/tddaemonclient/www/com.teardrop.WebInterface/" 
#define WEBINTERFACE_ROOT_SIZE 14

using namespace std;

static string newQuery(string query,string engines,string limit) {
	list<string> lse;
	tdParam tdp;
  engineResults *er = new engineResults();
  er->setQuery(query);
  er->setLimit(atol(limit.c_str()));
  size_t pos;
  while((pos = engines.find(","))!=string::npos) {
  	er->addEngine(engines.substr(0,pos));
  	engines = engines.substr(pos+1);
  }
  er->addEngine(engines);
  metaRank mr(er,&tdp);
  mr.startParsing();
  mr.joinAll();
  string pre = "{\"results\":[{";
  string text = string("\"num\":\"{num}\",") +
  							  "\"engines\":\"{engines}\"," +
  							  "\"title\":\"{title}\"," +
  							  "\"url\":\"{url}\"," +
  							  "\"img\":\"{img}\"," +
  							  "\"abstract\":\"{abstract}\"";
  string post = "}]}";
  return mr.getString(text,"/","},{",pre,post,true);
}

/*
 * This callback function is attached to the "/" and "/abc.html" URIs,
 * thus is acting as "index.html" file. It shows a bunch of links
 * to other URIs, and allows to change the value of program's
 * internal variable. The pointer to that variable is passed to the
 * callback function as arg->user_data.
 */
static void
show_index(struct shttpd_arg *arg)
{
	//int		*p = (int *) arg->user_data;	/* integer passed to us */
	//char		value[20];
	const char	*request_method, *query_string, *request_uri, *cookie_string;//*host,;

	request_method = shttpd_get_env(arg, "REQUEST_METHOD");
	request_uri = shttpd_get_env(arg, "REQUEST_URI");
	query_string = shttpd_get_env(arg, "QUERY_STRING");
	cookie_string = shttpd_get_header(arg, "Cookie");

	cout << request_uri << endl;
	
	if(!cookie_string) {
		shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
		shttpd_printf(arg, "%s", "Set-Cookie: query=0192825;\r\n");
	}


	shttpd_printf(arg, "%s",
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
		"<html><body><h1>");
	if(cookie_string)
		shttpd_printf(arg, "%s",cookie_string);
	else
		shttpd_printf(arg, "%s","YOU SHOULD HAVE A COOOKIIIE");
	shttpd_printf(arg, "%s", "</h1></body></html>");
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}
/*
 * This callback function is used to get the JSON of the engines tree
 */
static void
show_tree(struct shttpd_arg *arg)
{
	const char *request_uri = shttpd_get_env(arg, "REQUEST_URI");
	cout << request_uri << endl;
	shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
	shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");
	
	xmlFile xf;
	string config_path;
	
	if (selectFile::find("config",".xml",config_path) && xf.openFile(config_path + "config.xml")) {
		
		nodeDoc ndCateg(&xf,"category");
		shttpd_printf(arg, "%s", "{\"categories\":[");
		while(ndCateg.isValid()) {
			//Prints the category name
			shttpd_printf(arg, "%s", "{\"name\":\"");
			shttpd_printf(arg, "%s", ndCateg.getAttributeValueByName("name").c_str());
			shttpd_printf(arg, "%s", "\",\"icon\":\"folder.jpg\",\"engines\":[");
			nodeDoc ndEngine(&xf,"engine",ndCateg);
			while(ndEngine.isValid()) {
				shttpd_printf(arg, "%s", "{\"name\":\"");
				shttpd_printf(arg, "%s", ndEngine.getAttributeValueByName("name").c_str());
				string filename = ndEngine.getAttributeValueByName("name");
				string path;
				//QIcon *ic;

				shttpd_printf(arg, "%s", "\",\"icon\":\"");
				if (selectFile::find(filename,".png",path)) {
					shttpd_printf(arg, "%s", string(path + filename + ".png").c_str());
				} else {
					shttpd_printf(arg, "%s", string("xmag.png").c_str());
				}
				shttpd_printf(arg, "%s", "\",\"title\":\"");
				
				xmlEngine xe;
				if (selectFile::find(filename,".xml",path) && xe.openEngine(filename)) {
					shttpd_printf(arg, "%s", xe.getName().c_str());
				} else {
					// The file we a looking for was not found.
					//cerr << "The file " << filename << ".xml was not found." << endl;
				}
				shttpd_printf(arg, "%s", "\"}");
				ndEngine.next();
				if (ndEngine.isValid()) shttpd_printf(arg, "%s", ",");
			}
			shttpd_printf(arg, "%s", "]}");
			ndCateg.next();
			if (ndCateg.isValid()) shttpd_printf(arg, "%s", ",");
		}
		shttpd_printf(arg, "%s", "]}");
	} else {
		shttpd_printf(arg, "%s", "Teardrop configuration file could not be found.");
	}
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

/*
 * This callback function is used to show how to handle 404 error
 */
static void
show_404(struct shttpd_arg *arg)
{
	shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
	shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");
	//shttpd_printf(arg, "%s", "Oops. File not found! ");
	shttpd_printf(arg, "%s%s%s", "Oops. File", shttpd_get_env(arg, "REQUEST_URI") , " not found! ");
	shttpd_printf(arg, "%s", "This is a custom error handler.");
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

static void query_process(struct shttpd_arg *arg) {
	const char	*s;//, *path = "uploaded.txt";
	struct state {
		unsigned long int cl;		/* Content-Length	*/
		size_t	nread;		/* Number of bytes read	*/
		string	buffer;
	} *state;

	/* If the connection was broken prematurely, cleanup */
	if (arg->flags & SHTTPD_CONNECTION_ERROR && arg->state) {
		delete (struct state *)arg->state;
	} else if ((s = shttpd_get_header(arg, "Content-Length")) == NULL) {
		shttpd_printf(arg, "HTTP/1.0 411 Length Required\n\n");
		arg->flags |= SHTTPD_END_OF_OUTPUT;
	} else if (arg->state == NULL) {
		/* New request. Allocate a state structure, and open a file */
		arg->state = state = new (struct state);
		state->cl = strtoul(s, NULL, 10);
		state->buffer = "";
		state->nread = 0;
		shttpd_printf(arg, "HTTP/1.0 200 OK\n"
			"Content-Type: text/plain\n\n");
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
			regExp r("query=(.*);engines=([^;]*);limit=([1-9][0-9]{1,3})");
			r.newPage(state->buffer);
			if (!r.endOfMatch()) {
				//r.next();
        //shttpd_printf(arg, "Written %d bytes:\n%s\n%s\n%s",
			  //  state->nread,r.getMatch(1).c_str(),r.getMatch(2).c_str(),r.getMatch(3).c_str());
			  shttpd_printf(arg, "%s",
			    newQuery(r.getMatch(1),r.getMatch(2),r.getMatch(3)).c_str());
      } else {
      	shttpd_printf(arg, "You should not try to hack that soft ;-)\n");
			}
      
			
			//shttpd_printf(arg, "Written %d bytes: %s",
			//    state->nread, state->buffer.c_str());
			delete state;
			arg->flags |= SHTTPD_END_OF_OUTPUT;
		}
	}
}

/*
 * This callback function is used to show the web interface
 */
static void
show_wi(struct shttpd_arg *arg)
{
	string filename = WEBINTERFACE_PATH + string(shttpd_get_env(arg, "REQUEST_URI")).erase(0,WEBINTERFACE_ROOT_SIZE);
	FILE *fp = fopen(filename.c_str(),"rb");
	cerr << filename << endl;
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

int main(int argc, char *argv[])
{
	int			data = 1234567;
	struct shttpd_ctx	*ctx;
	
	/* Get rid of warnings */
	argc = argc;
	argv = argv;

	signal(SIGPIPE, SIG_IGN);

	/*
	 * Initialize SHTTPD context.
	 * Attach folder c:\ to the URL /my_c  (for windows), and
	 * /etc/ to URL /my_etc (for UNIX). These are Apache-like aliases.
	 * Set WWW root to current directory.
	 * Start listening on ports 8080 and 8081
	 */
	ctx = shttpd_init();
	shttpd_set_option(ctx, "ports", "8080");

	shttpd_register_uri(ctx, "/", &show_index, (void *) &data);
	shttpd_register_uri(ctx, "/services/request_tree", &show_tree, (void *) &data);
	shttpd_register_uri(ctx, "/services/query_post", &query_process, (void *) &data);
	shttpd_register_uri(ctx, WEBINTERFACE_URI, &show_wi, NULL);

	shttpd_handle_error(ctx, 404, show_404, NULL);

	/* Serve connections infinitely until someone kills us */
	for (;;)
		shttpd_poll(ctx, 1000);

	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);

	return (EXIT_SUCCESS);
}
