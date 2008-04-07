package com.teardrop.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.RootPanel;
import com.gwtext.client.widgets.tree.*;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.core.EventObject;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";
	  private TreePanel engTree = new EngineTree();
	  private Button searchButton = new Button(SEARCH_BUTTON_DEFAULT_TEXT,new ButtonListenerAdapter() {  
	      public void onClick(Button button, EventObject e) {
	    	  //Handle the button click
		      }  
		    });

	  /**
	   * Entry point for this simple application. Currently, we build the
	   * application's form and wait for events.
	   */
	  public void onModuleLoad() {
	    initializeMainForm();
	  }

	  /**
	   * Initialize the main form's layout and content.
	   */
	  private void initializeMainForm() {
	    searchButton.setStyleName("JSON-SearchButton");
	    searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);

	    // Find out where the host page wants the button.
	    //
	    RootPanel searchButtonSlot = RootPanel.get("search");
	    if (searchButtonSlot == null) {
	      Window.alert("Please define a container element whose id is 'search'");
	      return;
	    }

	    // Find out where the host page wants the tree view.
	    //
	    RootPanel treeViewSlot = RootPanel.get("engineTree");
	    if (treeViewSlot == null) {
	      Window.alert("Please define a container element whose id is 'tree'");
	      return;
	    }

	    // Add both widgets.
	    searchButtonSlot.add(searchButton);
	    treeViewSlot.add(engTree);
	  }
}
