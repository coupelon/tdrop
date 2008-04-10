package com.teardrop.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";
	  private EngineTree engTree = new EngineTree();
	  private TextBox queryText = new TextBox();
	  private QueryButton searchButton = new QueryButton(SEARCH_BUTTON_DEFAULT_TEXT,engTree,queryText);

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
	    
	    // Find out where the host page wants the query text box.
	    //
	    RootPanel queryTextSlot = RootPanel.get("engineTree");
	    if (queryTextSlot == null) {
	      Window.alert("Please define a container element whose id is 'query'");
	      return;
	    }

	    // Add both widgets.
	    searchButtonSlot.add(searchButton);
	    treeViewSlot.add(engTree);
	    queryTextSlot.add(queryText);
	  }
}