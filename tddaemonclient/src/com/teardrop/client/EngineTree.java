/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

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
import com.gwtext.client.core.EventObject;
import com.gwtext.client.data.Node;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Toolbar;
import com.gwtext.client.widgets.ToolbarButton;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
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
	
	final TabPanel centerPanel;
	
	public EngineTree(TabPanel centerPanel) {
		super();
		this.centerPanel = centerPanel;
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
	    loadEngineTree();
	    setVisible(true);
	    setPaddings(5);
	    
	    Toolbar toolbar = new Toolbar();
		ToolbarButton editTree = new ToolbarButton("Edit", new ButtonListenerAdapter() {  
			public void onClick(final Button innerButton, EventObject e) {  
				new EditTree(EngineTree.this.centerPanel, EngineTree.this);
			}  
		});
		editTree.setCls("x-btn-text-icon edit_tree");
		editTree.setTooltip("Edit the engines");
		toolbar.addButton(editTree);
		setTopToolbar(toolbar);
	}

	/*
	   * Add the object presented by the JSONValue as a children to the requested
	   * TreeItem.
	   */
	  private void generateEngineTree(TreeNode treeItem, JSONValue jsonValue) {
		while (treeItem.getChildNodes().length > 0) {
			treeItem.removeChild(treeItem.getLastChild());
		}
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
	  public void loadEngineTree() {
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
	    	      } else if (403 == response.getStatusCode()) {
	    	  			new Login(new Login.LoginCallback() {
							public void onCloseRun() {
			    	  			loadEngineTree();
							}
	    	  			});
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
