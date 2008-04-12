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
