/** Teardrop - main.cpp

@brief This is the console interface of Teardrop.

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 09/07/2006

# Copyright (C) 2006 Olivier COUPELON

# This file is part of Teardrop.
# 
# Teardrop is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# Teardrop is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Teardrop; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <iostream>
#include "pageParser.h"
#include "metaRank.h"
#include "openSave.h"
#include "tdParam.h"
#include "tddaemon.h"

#include <getopt.h>

using namespace std;

void showHelp() {
    cerr << "Usage : teardrop -q query [OPTIONS] engines" << endl;
    cerr << "Command line version of the meta search engine"<< endl << endl;

    cerr << "\t-q,--query=TEXT\t\tThe query TEXT you want to submit." << endl;
    cerr << "\t-l,--limit=NUMBER\t\tThe maximum number of results per engines you want." << endl;
    cerr << "\t-p,--print=FORMAT\t\tThe output format. Use {fieldname} to display the value of the given field." << endl;
    cerr << "\t-f,--format=FORMAT\t\tUsed with -o. Target's format (csv,html,tdp)." << endl;
    cerr << "\t-o,--output=FILENAME\t\tUsed with -f. The filename you want your query to be saved to." << endl;
    cerr << "\t-a,--proxy-address=ADDRESS\t\tAddress of the proxy you want to use." << endl;
    cerr << "\t-t,--proxy-port=NUMBER\t\tPort of the proxy you want to use." << endl;
    cerr << "\t-y,--proxy-type=TEXT\t\tType of the proxy you want to use. Defaults to HTTP. Other option is SOCKS5." << endl;
    cerr << "\t-d,--debug\t\tOutput every retrieved page source code. Useful to debug/create engines." << endl;
}

int main(int argc, char *argv[]) {
    /*outstream o;
    errstream e;*/
    int c;
    string query = "";
    bool query_flag = false;
    string output = "";
    bool output_flag = false;
    string format = "";
    bool format_flag = false;
    string limit = "-1";
    bool echo_flag = false;
    bool daemonize = false;
    string print = "{num}\t{engines}\n\tTitre: {title}\n\tAddress: {url}\nAbstract: {abstract}\n";
    string delim = "/";
    tdParam tdp;
    string proxy_address = "";
    string proxy_port = "0";
    string proxy_type = "HTTP";
    engineResults *er = new engineResults();

    struct option long_options[] = {
        {"limit", required_argument, 0, 'l'},
        {"query", required_argument, 0, 'q'},
        {"print", required_argument, 0, 'p'},
        {"format", required_argument, 0, 'f'},
        {"output", required_argument, 0, 'o'},
        {"options", required_argument, 0, 'i'},
        {"proxy-address", required_argument, 0, 'a'},
        {"proxy-port", required_argument, 0, 't'},
        {"proxy-type", required_argument, 0, 'y'},
        {"debug", no_argument, 0, 'd'},
        {0, 0, 0, 0}
    };
    
    while (1) {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long (argc, argv, "f:o:i:p:l:q:da:t:y:D",
                        long_options, &option_index);
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c)
        {
            case 'l':
                limit = optarg;
                break;
            case 'o':
                output = optarg;
                output_flag = true;
                break;
            case 'f':
                format = optarg;
                format_flag = true;
                break;
            case 'p':
                print = optarg;
                break;
            case 'q':
                query = optarg;
                query_flag = true;
                break;
            case 'd':
                echo_flag = true;
                break;
            case 'a':
                proxy_address = optarg;
                break;
            case 't':
                proxy_port = optarg;
                break;
            case 'y':
                proxy_type = optarg;
                break;
            case 'D':
            	daemonize = true;
            	break;
            case 'i':
                {//Parsing options
                regExp r("([0-9a-zA-Z_]*)=([0-9a-zA-Z_]*)");
                string st(optarg);
                for(r.newPage(st); !r.endOfMatch(); r.next()) {
                  er->addOption(r.getMatch(1),r.getMatch(2));
                }}
                break;
            default:
                showHelp();
                delete er;
                return -1;
        }
    }
    
    if (optind < argc && query_flag && ((output_flag && format_flag)||(!output_flag && !format_flag)))
    {
        list<string> lse;
        er->setQuery(query);
        er->setLimit(atol(limit.c_str()));
        while (optind < argc) {
            er->addEngine(argv[optind++]);
        }
        
        tdp.setEcho(echo_flag);
        tdp.setProxy(proxy_address,proxy_port,proxy_type);
        
        if (daemonize) {
        	TdDaemon daemon;
        	daemon.launchDaemon(&tdp);
        } else {
	        metaRank mr(er,&tdp);
	        mr.startParsing();
	        mr.joinAll();
	        //mr.startRanking();
	        if (output_flag) {
	            if (format == "tdp" )
	                openSave::xmlSave(output, query, lse, limit, mr.getResults());
	            else if (format == "html")
	                openSave::htmlExport(output, query, lse, limit, mr.getResults());
	            else if (format == "csv")
	                openSave::csvExport(output, mr.getResults());
	        } else mr.toString(print,delim);
        }
    } else {
      showHelp();
      delete er;
    }
    return 0;
}
