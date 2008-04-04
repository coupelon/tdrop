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

#define WEBINTERFACE_URI "/WebInterface/*"
#define WEBINTERFACE_PATH "/home/olivier/workspace/tddaemonclient/www/com.teardrop.WebInterface/" 
#define WEBINTERFACE_ROOT_SIZE 14

using namespace std;

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
	int		*p = (int *) arg->user_data;	/* integer passed to us */
	char		value[20];
	const char	*host, *request_method, *query_string, *request_uri, *cookie_string;

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
//    	shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
//    	filename = filename.substr(filename.size()-4,4);
//    	
//    	if (filename == "html")
//    		shttpd_printf(arg, "%s", "Content-Type: text/html\r\n\r\n");
//    	else
//    		shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");
//		shttpd_printf(arg, "%s", "\r\n");
		
//    	char *buffer = new char[1025];
//    	while (!feof(fp)) {
//	    	int nb = fread(buffer,1,1024,fp);
//	    	buffer[nb] = 0;
//	    	shttpd_printf(arg,"%.1024s", buffer);
//    	}
//    	delete[] buffer;
//    	fclose(fp);
//		arg->flags |= SHTTPD_END_OF_OUTPUT;
		
		int state = (int) arg->state;
	
		if (state == 0) {
			shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n\r\n");
		} else {
			fseek(fp,state,SEEK_SET);
		}
		char b;
		//cerr << "\t" << arg->out.num_bytes << " " << arg->out.len << endl;
		while (arg->out.num_bytes < arg->out.len && !feof(fp)) {
			int nb = fread(&b,1,1,fp);
			arg->out.buf[arg->out.num_bytes] = b;
			arg->out.num_bytes++;
			state++;
		}
		//cerr << "\t" << arg->out.num_bytes << " " << arg->out.len << endl;
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
	//shttpd_set_option(ctx, "ssl_cert", "shttpd.pem");
	//shttpd_set_option(ctx, "aliases", ALIAS_URI "=" ALIAS_DIR);
	shttpd_set_option(ctx, "ports", "8080");

	shttpd_register_uri(ctx, "/", &show_index, (void *) &data);
	shttpd_register_uri(ctx, "/request_tree", &show_tree, (void *) &data);
	shttpd_register_uri(ctx, WEBINTERFACE_URI, &show_wi, NULL);
	//shttpd_register_uri(ctx, "/search", &show_results, (void *) &data);

	shttpd_handle_error(ctx, 404, show_404, NULL);

	/* Serve connections infinitely until someone kills us */
	for (;;)
		shttpd_poll(ctx, 1000);

	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);

	return (EXIT_SUCCESS);
}
