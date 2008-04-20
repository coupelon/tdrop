package com.teardrop.client;

import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.*;  
import com.gwtext.client.widgets.event.ButtonListenerAdapter;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText, CycleButton limitButton,TabPanel centerPanel, Panel progressPanel) {
		super(text, new OnClickAdapter(engTree, queryText,limitButton,centerPanel,progressPanel));
		setCls("x-btn-text-icon btn-search-icon");
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		EngineTree engTree;
		TextBox queryText;
		CycleButton limitButton;
		TabPanel centerPanel;
		Panel progressPanel;
		public OnClickAdapter(EngineTree engTree, TextBox queryText, CycleButton limitButton, TabPanel centerPanel, Panel progressPanel) {
			this.engTree = engTree;
			this.queryText = queryText;
			this.limitButton = limitButton;
			this.centerPanel = centerPanel;
			this.progressPanel = progressPanel;
		}
		
		public void onClick(Button button, EventObject e) {
			new PerformSearch(engTree, queryText,limitButton,centerPanel,progressPanel);
        }
	}
}
