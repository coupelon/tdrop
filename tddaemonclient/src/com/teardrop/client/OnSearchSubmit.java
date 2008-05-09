package com.teardrop.client;

import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.event.KeyListener;

public class OnSearchSubmit extends ButtonListenerAdapter implements KeyListener {
	EngineTree engTree;
	TextField queryText;
	CycleButton limitButton;
	TabPanel centerPanel;
	public OnSearchSubmit(EngineTree engTree, TextField queryText, CycleButton limitButton, TabPanel centerPanel) {
		this.engTree = engTree;
		this.queryText = queryText;
		this.limitButton = limitButton;
		this.centerPanel = centerPanel;
	}
	
	public void onClick(Button button, EventObject e) {
		submit();
    }
	
	public void onKey(int key,EventObject e) {
		submit();
	}
	
	public void submit() {
		new PerformSearch(engTree, queryText,limitButton,centerPanel);
	}
}
