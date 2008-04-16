package com.teardrop.client;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.json.client.JSONArray;
import com.google.gwt.json.client.JSONParser;
import com.google.gwt.json.client.JSONValue;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.data.Node;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.layout.RowLayout;
import com.gwtext.client.widgets.tree.TreeNode;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText, CycleButton limitButton,TabPanel centerPanel) {
		super(text, new OnClickAdapter(engTree, queryText,limitButton,centerPanel));
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		private static final String DEFAULT_SEARCH_URL = "/services/query_post";
		private static final String DEFAULT_NEXT_URL = "/services/get_next_results";
		EngineTree engTree;
		TextBox queryText;
		CycleButton limitButton;
		TabPanel centerPanel;
		Panel resultsPanel;
		public OnClickAdapter(EngineTree engTree, TextBox queryText, CycleButton limitButton, TabPanel centerPanel) {
			this.engTree = engTree;
			this.queryText = queryText;
			this.limitButton = limitButton;
			this.centerPanel = centerPanel;
		}
		
		//To circumvent the three checks bug...
		public String getChecked() {
			String checkedNodeString = "";
			TreeNode n = engTree.getRootNode();
			Node[] tnChild = n.getChildNodes();
			for (int i = 0; i < tnChild.length; ++i) {
				Node[] tnEng = tnChild[i].getChildNodes();
				for (int j=0; j < tnEng.length; ++j) {
					if (((TreeNode) tnEng[j]).getUI().isChecked())
						checkedNodeString += (checkedNodeString.equals("")?"":",") + tnEng[j].getAttribute("name");
				}
			}
			return checkedNodeString;
		}
		
		public void onClick(Button button, EventObject e) {
//			String checkedNodeString = "";
//			TreeNode[] checkedNode = engTree.getChecked();
//			for(int i = 0; i < checkedNode.length; ++i) {
//				if (checkedNode[i].getAttribute("name") != null) {
//					checkedNodeString += (checkedNodeString.equals("")?"":",") + checkedNode[i].getAttribute("name");
//				}
//			}
			String checkedNodeString = getChecked();
			Window.alert(checkedNodeString);
			
			//Workaround to a bug:
			String limit = limitButton.getText().substring(limitButton.getPrependText().length());
			//String limit = limitButton.getActiveItem().getText()
			doPostURL("query=" + queryText.getText() + ";engines=" + checkedNodeString + ";limit=" + limit,DEFAULT_SEARCH_URL);
			
			resultsPanel = new Panel();
			resultsPanel.setLayout(new RowLayout());
		    resultsPanel.setAutoScroll(true);
		    resultsPanel.setTitle(URL.encodeComponent(queryText.getText()));
		    resultsPanel.setClosable(true);
			centerPanel.add(resultsPanel);
			centerPanel.activate(centerPanel.getItems().length-1);
        }
		
		/*
		 * Fetch the requested URL.
		 */
		private void doPostURL(String post, String url) {
		    RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(url));
		    try {
		    	builder.setHeader("Content-Length", String.valueOf(post.length()));
		    	builder.sendRequest(post, new RequestCallback() {
		    	    public void onError(Request request, Throwable exception) {
		    	    	Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
		    	    }
	
		    	    public void onResponseReceived(Request request, Response response) {
		    	      if (200 == response.getStatusCode()) {
		    	  	      JSONValue jsonValue = JSONParser.parse(response.getText());
		    	  	      showResults(jsonValue);
		    	      } else {
		    	    	  Window.alert(response.getStatusText());
		    	      }
		    	    }       
		    	  });
		    	} catch (RequestException e) {
		    	  // Couldn't connect to server        
		    	}
		  }
		
		private void showResults(JSONValue jsonValue) {
			JSONValue results;
			resultsPanel.removeAll(true);
			results = JSONFunctions.getJSONSet(jsonValue,"preresults");
			if (results == null) {
				results = JSONFunctions.getJSONSet(jsonValue,"results");
			} else {
				doPostURL("",DEFAULT_NEXT_URL);
			}
		    if (results != null) {
		    	JSONArray resultsArray;
		    	if ((resultsArray = results.isArray()) != null) {
	    	      for (int i = 0; i < resultsArray.size(); ++i) {
	    	    	Panel newPanel = new Panel();
	    	    	newPanel.setAutoHeight(true);
	    	    	newPanel.setPaddings(5);
	    	    	newPanel.add(new HTML(URL.decodeComponent(JSONFunctions.getJSONSetValue(resultsArray.get(i), "title"))));
	    	    	newPanel.add(new HTML(URL.decodeComponent(JSONFunctions.getJSONSetValue(resultsArray.get(i), "abstract"))));
	    	    	newPanel.add(new HTML(URL.decodeComponent(JSONFunctions.getJSONSetValue(resultsArray.get(i), "img"))));
	    	    	newPanel.add(new HTML(URL.decodeComponent(JSONFunctions.getJSONSetValue(resultsArray.get(i), "engines"))));
	    	    	newPanel.add(new HTML(URL.decodeComponent(JSONFunctions.getJSONSetValue(resultsArray.get(i), "url"))));
	    	    	resultsPanel.add(newPanel);
	    	      }
	    	    }
		    }
		}
	}
}
