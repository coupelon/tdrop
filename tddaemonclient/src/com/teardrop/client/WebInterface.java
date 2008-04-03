package com.teardrop.client;

import java.util.Iterator;
import java.util.Set;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.HTTPRequest;
import com.google.gwt.user.client.ResponseTextHandler;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Tree;
import com.google.gwt.user.client.ui.TreeItem;
import com.google.gwt.user.client.ui.Widget;

import com.google.gwt.json.client.*;;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  /**
	   * Class for handling the response text associated with a request for a JSON
	   * object.
	   * 
	   */
	  private class JSONResponseTextHandler implements ResponseTextHandler {
	    public void onCompletion(String responseText) {
	      try {
	        JSONValue jsonValue = JSONParser.parse(responseText);
	        displayJSONObject(jsonValue);
	      } catch (JSONException e) {
	        displayError(responseText);
	      }
	      searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    }
	  }

	  /*
	   * Class for handling the fetch button's click event.
	   */
	  private class SearchButtonClickListener implements ClickListener {
	    public void onClick(Widget sender) {
	      jsonTree.setVisible(false);
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

	  private Tree jsonTree = new Tree();

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
	  private void addChildren(TreeItem treeItem, JSONValue jsonValue) {
	    JSONArray jsonArray;
	    JSONObject jsonObject;
	    JSONString jsonString;
	    
	    if ((jsonArray = jsonValue.isArray()) != null) {
	      for (int i = 0; i < jsonArray.size(); ++i) {
	        TreeItem child = treeItem.addItem(getChildText("["
	            + Integer.toString(i) + "]"));
	        addChildren(child, jsonArray.get(i));
	      }
	    } else if ((jsonObject = jsonValue.isObject()) != null) {
	      Set keys = jsonObject.keySet();
	      for (Iterator iter = keys.iterator(); iter.hasNext();) {
	        String key = (String) iter.next();
	        TreeItem child = treeItem.addItem(getChildText(key));
	        addChildren(child, jsonObject.get(key));
	      }
	    } else if ((jsonString = jsonValue.isString()) != null) {
	      // Use stringValue instead of toString() because we don't want escaping
	      treeItem.addItem(jsonString.stringValue());
	    } else {
	      // JSONBoolean, JSONNumber, and JSONNull work well with toString().
	      treeItem.addItem(getChildText(jsonValue.toString()));
	    }
	  }

	  private void displayError(String responseText) {
	    jsonTree.removeItems();
	    jsonTree.setVisible(true);
	    TreeItem treeItem = jsonTree.addItem("Failed to parse JSON response");
	    treeItem.addItem(responseText);
	    treeItem.setStyleName("JSON-JSONResponseObject");
	    treeItem.setState(true);
	  }

	  /*
	   * Update the treeview of a JSON object.
	   */
	  private void displayJSONObject(JSONValue jsonValue) {
	    jsonTree.removeItems();
	    jsonTree.setVisible(true);
	    TreeItem treeItem = jsonTree.addItem("JSON Response");
	    addChildren(treeItem, jsonValue);
	    treeItem.setStyleName("JSON-JSONResponseObject");
	    treeItem.setState(true);
	  }

	  /*
	   * Fetch the requested URL.
	   */
	  private void doFetchURL() {
	    searchButton.setText(SEARCH_BUTTON_WAITING_TEXT);
	    if (!HTTPRequest.asyncGet(DEFAULT_SEARCH_URL, new JSONResponseTextHandler())) {
	      // Reset the caption.
	      //
	      searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    }
	  }

	  /*
	   * Causes the text of child elements to wrap.
	   */
	  private String getChildText(String text) {
	    return "<span style='white-space:normal'>" + text + "</span>";
	  }

	  /**
	   * Initialize the main form's layout and content.
	   */
	  private void initializeMainForm() {
	    searchButton.setStyleName("JSON-SearchButton");
	    searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    searchButton.addClickListener(new SearchButtonClickListener());

	    // Avoids showing an "empty" cell
	    jsonTree.setVisible(false);

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
	    treeViewSlot.add(jsonTree);
	  }
}
