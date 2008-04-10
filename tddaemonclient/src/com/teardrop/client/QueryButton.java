package com.teardrop.client;

import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText) {
		super(text, new ButtonListenerAdapter() {  
		      public void onClick(Button button, EventObject e) {
		    	  //Handle the button click
			      }  
			    });
	}
}
