package com.teardrop.client;

import java.util.Iterator;
import java.util.Set;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.json.client.JSONArray;
import com.google.gwt.json.client.JSONException;
import com.google.gwt.json.client.JSONObject;
import com.google.gwt.json.client.JSONParser;
import com.google.gwt.json.client.JSONString;
import com.google.gwt.json.client.JSONValue;
import com.gwtext.client.widgets.tree.TreeNode;
import com.gwtext.client.widgets.tree.TreePanel;

public class EngineTree extends TreePanel {

	private static final String DEFAULT_SEARCH_URL = "http://localhost:8080/services/request_tree";
	//private static final String DEFAULT_SEARCH_URL = "search-results.js";
	private static final String ICON_PATH = "teardrop/";
	
	
	public EngineTree() {
		super();
		// TODO Auto-generated constructor stub
		TreeNode aroot = new TreeNode("categories"); 
		setRootNode(aroot);
		// Avoids showing an "empty" cell
	    setHeaderAsText(true);
	    setTitle("Categories");
	    setCollapsible(true);
	    setHeight(400);  
	    setWidth(200);  
	    setAnimate(true);  
	    setEnableDD(false);  
	    setContainerScroll(true);  
	    setAutoScroll(true);  
	    setRootVisible(false);  
	    setFrame(true);
	    doFetchURL();
	    setVisible(true);
	}

	/*
	   * Add the object presented by the JSONValue as a children to the requested
	   * TreeItem.
	   */
	  private void generateEngineTree(TreeNode treeItem, JSONValue jsonValue) {	    
	    JSONValue categ;
	    if ((categ = getJSONSet(jsonValue,"categories")) != null) {
	    	JSONArray categArray;
	    	if ((categArray = categ.isArray()) != null) {
    	      for (int i = 0; i < categArray.size(); ++i) {
    	    	TreeNode treeCat = new TreeNode(getJSONSetValue(categArray.get(i), "name"));
    	    	treeCat.setIcon(ICON_PATH + getJSONSetValue(categArray.get(i), "icon"));
    	    	treeCat.setChecked(false);
    	    	treeItem.appendChild(treeCat);
    	        JSONValue engin;
    	        if ((engin = getJSONSet(categArray.get(i),"engines")) != null) {
	  	        	JSONArray enginArray;
	  	        	if ((enginArray = engin.isArray()) != null) {
	  	    	      for (int j = 0; j < enginArray.size(); ++j) {
	  	    	    	TreeNode treeEng;
	  	    	    	if (getJSONSetValue(enginArray.get(j),"title").equals(""))
	  	    	    		treeEng = new TreeNode(getJSONSetValue(enginArray.get(j),"name"));
	  	    	    	else 
	  	    	    		treeEng = new TreeNode(getJSONSetValue(enginArray.get(j),"title"));
	  	    	    	treeEng.setIcon(ICON_PATH + getJSONSetValue(enginArray.get(j),"icon"));
	  	    	    	treeEng.setAttribute("name", getJSONSetValue(enginArray.get(j),"name"));
	  	    	    	treeEng.setChecked(false);
	  	    	    	treeCat.appendChild(treeEng);
	  	    	      }
	  	    	    }
    	        }
    	      }
    	    }
	    }
	  }
	  
	  private String getJSONSetValue(JSONValue jsonValue, String name) {
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
	  
	  private JSONValue getJSONSet(JSONValue jsonValue, String name) {
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

	  private void displayError(String responseText) {
	    clear();
	    setVisible(true);
	    TreeNode treeItem = new TreeNode("Failed to parse JSON response"); 
	    setRootNode(treeItem);
	  }


	  /*
	   * Fetch the requested URL.
	   */
	  private void doFetchURL() {
	    RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, URL.encode(DEFAULT_SEARCH_URL));
	    try {
	    	builder.sendRequest(null, new RequestCallback() {
	    	    public void onError(Request request, Throwable exception) {
	    	    	displayError("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
	    	    }

	    	    public void onResponseReceived(Request request, Response response) {
	    	      if (200 == response.getStatusCode()) {
	    	    	  try {
	    	  	        JSONValue jsonValue = JSONParser.parse(response.getText());
	    	  	      generateEngineTree(getRootNode(), jsonValue);
	    	  	      } catch (JSONException e) {
	    	  	        displayError(response.getText());
	    	  	      }
	    	          // Process the response in response.getText()
	    	      } else {
	    	    	    displayError(response.getStatusText());
	    	      }
	    	    }       
	    	  });
	    	} catch (RequestException e) {
	    	  // Couldn't connect to server        
	    	}
	  }
}
