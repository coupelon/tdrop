package com.teardrop.client;

import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.*;  
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.form.TextField;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextField queryText, CycleButton limitButton,TabPanel centerPanel) {
		super(text, new OnClickAdapter(engTree, queryText,limitButton,centerPanel));
		setCls("x-btn-text-icon btn-search-icon");
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		EngineTree engTree;
		TextField queryText;
		CycleButton limitButton;
		TabPanel centerPanel;
		public OnClickAdapter(EngineTree engTree, TextField queryText, CycleButton limitButton, TabPanel centerPanel) {
			this.engTree = engTree;
			this.queryText = queryText;
			this.limitButton = limitButton;
			this.centerPanel = centerPanel;
		}
		
		public void onClick(Button button, EventObject e) {
			new PerformSearch(engTree, queryText,limitButton,centerPanel);
        }
	}
}
