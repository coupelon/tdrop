package com.teardrop.client;

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
		EngineTree engTree;
		TextBox queryText;
		public OnClickAdapter(EngineTree engTree, TextBox queryText) {
			this.engTree = engTree;
			this.queryText = queryText;
		}
		
		public void onClick(Button button, EventObject e) {
			TreeNode[] checkedNode = engTree.getChecked();
			String checkedNodeString = "engines=";
			for(int i = 0; i < checkedNode.length; ++i) {
				checkedNodeString += (i>0?",":"") + checkedNode[i].getAttribute("name");
			}
			Window.alert("query=" + queryText.getText() + checkedNodeString);
        }
		
	}
}
