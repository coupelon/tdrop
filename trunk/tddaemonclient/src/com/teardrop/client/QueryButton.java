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
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.tree.TreeNode;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText, CycleButton limitButton,Panel resultsPanel) {
		super(text, new OnClickAdapter(engTree, queryText,limitButton,resultsPanel));
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		private static final String DEFAULT_SEARCH_URL = "http://localhost:8080/services/query_post";
		private static final String DEFAULT_NEXT_URL = "http://localhost:8080/services/get_next_results";
		EngineTree engTree;
		TextBox queryText;
		CycleButton limitButton;
		Panel resultsPanel;
		public OnClickAdapter(EngineTree engTree, TextBox queryText, CycleButton limitButton, Panel resultsPanel) {
			this.engTree = engTree;
			this.queryText = queryText;
			this.limitButton = limitButton;
			this.resultsPanel = resultsPanel;
		}
		
		public void onClick(Button button, EventObject e) {
			TreeNode[] checkedNode = engTree.getChecked();
			String checkedNodeString = "";
			for(int i = 0; i < checkedNode.length; ++i) {
				if (checkedNode[i].getAttribute("name") != null) {
					checkedNodeString += (checkedNodeString.equals("")?"":",") + checkedNode[i].getAttribute("name");
				}
			}
			doPostURL("query=" + queryText.getText() + ";engines=" + checkedNodeString + ";limit=10",DEFAULT_SEARCH_URL);
			//limitButton.getActiveItem().getText();
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
