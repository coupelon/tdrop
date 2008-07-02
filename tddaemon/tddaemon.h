/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef DAEMON_H_
#define DAEMON_H_

#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>

#include <fstream>

#include "nodeDoc.h"
#include "xmlEngine.h"
#include "selectFile.h"
#include "regExp.h"
#include "metaRank.h"
#include "UIDSession.h"
#include "shttpd.h"
#include "users.h"

#define WEBINTERFACE_URI "/WebInterface/*"
#define WEBINTERFACE_PATH "/tddaemonclient/www/com.teardrop.WebInterface/"
#define WEBINTERFACE_ROOT_SIZE 14
#define WEBINTERFACE_ENGINES_ICONS_URI "/engines_icons/*"
#define WEBINTERFACE_ENGINES_ICONS selectFile::getHomeDirectory() + HOME_ICONS
#define WEBINTERFACE_ENGINES_ICONS_SIZE 15
#define NEXT_RESULTS "/services/get_next_results"
#define QUERY_POST "/services/query_post"
#define REQUEST_TREE "/services/request_tree"
#define EXPORT_CSV "/services/export_csv"
#define AVAILABLE_ENGINES "/config/get_available_engines"
#define UPDATE_URL "http://tdrop.googlecode.com/svn/trunk/tdengines/updates.xml"
#define SAVE_ENGINES "/config/save_engines"
#define AUTHENTICATE_USER "/login"
#define GET_USER_HISTORY "/get_user_history"
#define DEFAULT_HEADER "HTTP/1.0 200 OK\nContent-Type: text/plain\r\n"
#define DEFAULT_HEADER_NL "HTTP/1.0 200 OK\nContent-Type: text/plain\r\n\r\n"
#define CSV_HEADER_NL "HTTP/1.0 200 OK\nContent-Type: text/csv\r\n\r\n"

using namespace std;

class TdDaemon {

public:
  /**
   * This method launches the Teardrop daemon. It should never return.
   * @param tdp The teardrop configuration
   */
	void launchDaemon(tdParam *tdp);
	
private:
	static string createJSON(bool final, metaRank *mr);
	static string newQuery(struct shttpd_arg *arg, string query,string engines,string limit);
	static string get_next_results(struct shttpd_arg *arg);
	static string export_csv(struct shttpd_arg *arg);
	static void show_index(struct shttpd_arg *arg);
	static string show_tree();
	static void show_404(struct shttpd_arg *arg);
	static void query_process(struct shttpd_arg *arg);
	static void show_file(struct shttpd_arg *arg,string filename);
	static void show_engines_icons(struct shttpd_arg *arg);
	static void show_wi(struct shttpd_arg *arg);
	static string show_available_engines();
	static string save_config(string s);
	static string get_authenticate_user(struct shttpd_arg *arg, string user,string pass);

	//The teardrop parameters
	static tdParam *tdp;
	static users *clients;
	static UIDSession *searches;
};

#endif /*DAEMON_H_*/
