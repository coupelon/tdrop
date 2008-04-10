package com.teardrop.client;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.tree.TreeNode;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText) {
		super(text, new OnClickAdapter(engTree, queryText));
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		private static final String DEFAULT_SEARCH_URL = "http://localhost:8080/services/query_post";
		EngineTree engTree;
		TextBox queryText;
		public OnClickAdapter(EngineTree engTree, TextBox queryText) {
			this.engTree = engTree;
			this.queryText = queryText;
		}
		
		public void onClick(Button button, EventObject e) {
			TreeNode[] checkedNode = engTree.getChecked();
			String checkedNodeString = "";
			for(int i = 0; i < checkedNode.length; ++i) {
				if (checkedNode[i].getAttribute("name") != null) {
					checkedNodeString += (checkedNodeString.equals("")?"":",") + checkedNode[i].getAttribute("name");
				}
			}
			doPostURL("query=" + queryText.getText() + ";engines=" + checkedNodeString);
        }
		
		/*
		 * Fetch the requested URL.
		 */
		private void doPostURL(String post) {
		    RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(DEFAULT_SEARCH_URL));
		    try {
		    	builder.setHeader("Content-Length", String.valueOf(post.length()));
		    	builder.sendRequest(post, new RequestCallback() {
		    	    public void onError(Request request, Throwable exception) {
		    	    	Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
		    	    }
	
		    	    public void onResponseReceived(Request request, Response response) {
		    	      if (200 == response.getStatusCode()) {
		    	  	      Window.alert(response.getText());
		    	      } else {
		    	    	  Window.alert(response.getStatusText());
		    	      }
		    	    }       
		    	  });
		    	} catch (RequestException e) {
		    	  // Couldn't connect to server        
		    	}
		  }
		
	}
}
