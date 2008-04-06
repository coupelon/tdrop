package com.teardrop.client;

import java.util.Iterator;
import java.util.Set;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.http.client.*;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

import com.google.gwt.json.client.*;

import com.gwtext.client.widgets.tree.*;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  /*
	   * Class for handling the fetch button's click event.
	   */
	  private class SearchButtonClickListener implements ClickListener {
	    public void onClick(Widget sender) {
	      engTree.setVisible(false);
	      doFetchURL();
	    }
	  }

	  /*
	   * Default URL to use to fetch JSON objects. Note that the contents of this
	   * JSON result were as a result of requesting the following URL:
	   * 
	   * http://api.search.yahoo.com/ImageSearchService/V1/imageSearch?appid=YahooDemo&query=potato&results=2&output=json
	   * 
	   */
	  //private static final String DEFAULT_SEARCH_URL = "http://localhost:8080/request_tree";
	  private static final String DEFAULT_SEARCH_URL = "search-results.js";

	  /*
	   * Text displayed on the fetch button when we are in a default state.
	   */
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";

	  /*
	   * Text displayed on the fetch button when we are waiting for a JSON reply.
	   */
	  private static final String SEARCH_BUTTON_WAITING_TEXT = "Waiting for JSON Response...";

	  //private Tree jsonTree = new Tree();
	  
	  private TreePanel engTree = new TreePanel();

	  private Button searchButton = new Button();

	  /**
	   * Entry point for this simple application. Currently, we build the
	   * application's form and wait for events.
	   */
	  public void onModuleLoad() {
	    initializeMainForm();
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
      	    	treeCat.setIcon(getJSONSetValue(categArray.get(i), "icon"));
      	    	treeCat.setChecked(false);
      	    	treeItem.appendChild(treeCat);
      	        JSONValue engin;
      	        if ((engin = getJSONSet(categArray.get(i),"engines")) != null) {
      	        	JSONArray enginArray;
	  	        	if ((enginArray = engin.isArray()) != null) {
	  	    	      for (int j = 0; j < enginArray.size(); ++j) {
	  	    	    	TreeNode treeEng = new TreeNode(getJSONSetValue(enginArray.get(j),"title"));
	  	    	    	treeEng.setIcon(getJSONSetValue(enginArray.get(j),"icon"));
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
	    engTree.clear();
	    engTree.setVisible(true);
	    TreeNode treeItem = new TreeNode("Failed to parse JSON response"); 
	    engTree.setRootNode(treeItem);
	  }

	  /*
	   * Update the treeview of a JSON object.
	   */
	  private void displayJSONObject(JSONValue jsonValue) {
		engTree.setVisible(true);
	    //jsonTree.removeItems();
	    //jsonTree.setVisible(true);
	    //TreeItem treeItem = jsonTree.addItem("JSON Response");
	    generateEngineTree(engTree.getRootNode(), jsonValue);
	    //treeItem.setStyleName("JSON-JSONResponseObject");
	    //treeItem.setState(true);
	  }

	  /*
	   * Fetch the requested URL.
	   */
	  private void doFetchURL() {
	    searchButton.setText(SEARCH_BUTTON_WAITING_TEXT);
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
	    	  	        displayJSONObject(jsonValue);
	    	  	      } catch (JSONException e) {
	    	  	        displayError(response.getText());
	    	  	      }
	    	  	      searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    	          // Process the response in response.getText()
	    	      } else {
	    	    	    displayError(response.getStatusText());
						// Handle the error.  Can get the status text from response.getStatusText()
						searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    	      }
	    	    }       
	    	  });
	    	} catch (RequestException e) {
	    	  // Couldn't connect to server        
	    	}
	  }

	  /*
	   * Causes the text of child elements to wrap.
	   */
//	  private String getChildText(String text) {
//	    return "<span style='white-space:normal'>" + text + "</span>";
//	  }

	  /**
	   * Initialize the main form's layout and content.
	   */
	  private void initializeMainForm() {
	    searchButton.setStyleName("JSON-SearchButton");
	    searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    searchButton.addClickListener(new SearchButtonClickListener());

	    TreeNode aroot = new TreeNode("categories"); 
		engTree.setRootNode(aroot);
		// Avoids showing an "empty" cell
	    engTree.setVisible(false);
	    
	    engTree.setCollapsible(true);
	    engTree.setHeight(400);  
	    engTree.setWidth(200);  
	    engTree.setAnimate(true);  
	    engTree.setEnableDD(true);  
	    engTree.setContainerScroll(true);  
	    engTree.setAutoScroll(true);  
	    engTree.setRootVisible(false);  
	    engTree.setFrame(true);  

	    // Find out where the host page wants the button.
	    //
	    RootPanel searchButtonSlot = RootPanel.get("search");
	    if (searchButtonSlot == null) {
	      Window.alert("Please define a container element whose id is 'search'");
	      return;
	    }

	    // Find out where the host page wants the tree view.
	    //
	    RootPanel treeViewSlot = RootPanel.get("tree");
	    if (treeViewSlot == null) {
	      Window.alert("Please define a container element whose id is 'tree'");
	      return;
	    }

	    // Add both widgets.
	    //
	    searchButtonSlot.add(searchButton);
	    treeViewSlot.add(engTree);
	  }
}
