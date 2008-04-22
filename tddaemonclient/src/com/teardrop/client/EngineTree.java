package com.teardrop.client;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.json.client.JSONArray;
import com.google.gwt.json.client.JSONException;
import com.google.gwt.json.client.JSONParser;
import com.google.gwt.json.client.JSONValue;
import com.gwtext.client.data.Node;
import com.gwtext.client.widgets.tree.TreeNode;
import com.gwtext.client.widgets.tree.TreePanel;
import com.gwtext.client.widgets.tree.event.TreeNodeListener;
import com.gwtext.client.widgets.tree.event.TreeNodeListenerAdapter;

public class EngineTree extends TreePanel {

	private static final String DEFAULT_SEARCH_URL = "/services/request_tree";
	//private static final String DEFAULT_SEARCH_URL = "search-results.js";
	private static final String ICON_PATH = "imgs/";
	
	//This is the listener for the categories checkboxes of the tree
	private static final TreeNodeListener catListener = new TreeNodeListenerAdapter() {
		public void onCheckChanged(Node node, boolean checked) {
			for(int j = 0; j < node.getChildNodes().length; ++j) {
				((TreeNode)node.getChildNodes()[j]).getUI().toggleCheck(checked);
			}
		}
		public void onExpand(Node node) {
			for(int j = 0; j < node.getChildNodes().length; ++j) {
				((TreeNode)node.getChildNodes()[j]).getUI().toggleCheck(((TreeNode)node).getUI().isChecked());
			}
		}
	};
	
	//This is the listener for the engines checkboxes of the tree
	private static final TreeNodeListener engListener = new TreeNodeListenerAdapter() {
		public void onCheckChanged(Node node, boolean checked) {
			TreeNode parent = (TreeNode) node.getParentNode();
			if (!checked) {
				parent.getUI().toggleCheck(false);
			} else  if (!parent.getUI().isChecked()) {
				boolean notChecked = false;
				for(int j = 0; j < parent.getChildNodes().length; ++j) {
    				if (!((TreeNode)parent.getChildNodes()[j]).getUI().isChecked()) {
    					notChecked = true;
    					break;
    				}
    			}
				if (!notChecked) {
					//If every child is checked, we can toggle this node
					parent.getUI().toggleCheck(true);
				}
			}
		}
	};
	
	public EngineTree() {
		super();
		// TODO Auto-generated constructor stub
		TreeNode aroot = new TreeNode("categories"); 
		setRootNode(aroot);
		// Avoids showing an "empty" cell
	    setHeaderAsText(true);
	    setTitle("Categories");
	    setCollapsible(true);
	    setAnimate(true);  
	    setEnableDD(false);  
	    setContainerScroll(true);  
	    setAutoScroll(true);  
	    setRootVisible(false);
	    doFetchURL();
	    setVisible(true);
	    setPaddings(5);
	}

	/*
	   * Add the object presented by the JSONValue as a children to the requested
	   * TreeItem.
	   */
	  private void generateEngineTree(TreeNode treeItem, JSONValue jsonValue) {	    
	    JSONValue categ;
	    if ((categ = JSONFunctions.getJSONSet(jsonValue,"categories")) != null) {
	    	JSONArray categArray;
	    	if ((categArray = categ.isArray()) != null) {
    	      for (int i = 0; i < categArray.size(); ++i) {
    	    	TreeNode treeCat = new TreeNode(JSONFunctions.getJSONSetValue(categArray.get(i), "name"));
    	    	treeCat.setIcon(ICON_PATH + JSONFunctions.getJSONSetValue(categArray.get(i), "icon"));
    	    	treeCat.setChecked(false);
    	    	treeCat.addListener(catListener);
    	    	treeItem.appendChild(treeCat);
    	        JSONValue engin;
    	        if ((engin = JSONFunctions.getJSONSet(categArray.get(i),"engines")) != null) {
	  	        	JSONArray enginArray;
	  	        	if ((enginArray = engin.isArray()) != null) {
	  	    	      for (int j = 0; j < enginArray.size(); ++j) {
	  	    	    	TreeNode treeEng;
	  	    	    	if (JSONFunctions.getJSONSetValue(enginArray.get(j),"title").equals(""))
	  	    	    		treeEng = new TreeNode(JSONFunctions.getJSONSetValue(enginArray.get(j),"name"));
	  	    	    	else 
	  	    	    		treeEng = new TreeNode(JSONFunctions.getJSONSetValue(enginArray.get(j),"title"));
	  	    	    	treeEng.setIcon(JSONFunctions.getJSONSetValue(enginArray.get(j),"icon"));
	  	    	    	treeEng.setAttribute("name", JSONFunctions.getJSONSetValue(enginArray.get(j),"name"));
	  	    	    	treeEng.setChecked(false);
	  	    	    	treeEng.addListener(engListener);
	  	    	    	treeCat.appendChild(treeEng);
	  	    	      }
	  	    	    }
    	        }
    	      }
    	    }
	    }
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
