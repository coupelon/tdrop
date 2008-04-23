/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import java.util.Iterator;
import java.util.Set;

import com.google.gwt.json.client.JSONObject;
import com.google.gwt.json.client.JSONString;
import com.google.gwt.json.client.JSONValue;

public class JSONFunctions {
	  public static String getJSONSetValue(JSONValue jsonValue, String name) {
		    JSONObject jsonObject;
		    JSONString jsonString;
			if ((jsonObject = jsonValue.isObject()) != null) {
		      Set keys = jsonObject.keySet();
		      for (Iterator iter = keys.iterator(); iter.hasNext();) {
		        String key = (String) iter.next();
		        if(key.equals(name))
			        if ((jsonString = jsonObject.get(key).isString()) != null) {
			  	      return jsonString.stringValue();
			  	    }
		      }
		    }
			return null;
		  }
		  
	  public static JSONValue getJSONSet(JSONValue jsonValue, String name) {
			JSONObject jsonObject;
			if ((jsonObject = jsonValue.isObject()) != null) {
		      Set keys = jsonObject.keySet();
		      for (Iterator iter = keys.iterator(); iter.hasNext();) {
		        String key = (String) iter.next();
		        if(key.equals(name))
		        	return jsonObject.get(key);
		      }
		    }
			return null;
		  }
}
