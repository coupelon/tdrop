package com.teardrop.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.RegionPosition;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Viewport;
import com.gwtext.client.widgets.layout.BorderLayout;
import com.gwtext.client.widgets.layout.BorderLayoutData;
import com.gwtext.client.widgets.layout.FitLayout;
import com.gwtext.client.widgets.layout.VerticalLayout;
import com.gwtext.client.widgets.menu.CheckItem;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";
	  private EngineTree engTree = new EngineTree();
	  private TextBox queryText = new TextBox();
	  private CycleButton limitButton = new CycleButton();
	  private TabPanel centerPanel = new TabPanel();
	  private Panel progressPanel = new Panel();
	  private QueryButton searchButton = new QueryButton(SEARCH_BUTTON_DEFAULT_TEXT,engTree,queryText,limitButton,centerPanel,progressPanel);

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
	    searchButton.setText(SEARCH_BUTTON_DEFAULT_TEXT);
	    
	    limitButton.setShowText(true);  
	    limitButton.setPrependText("Results per engines: ");
	    limitButton.addItem(new CheckItem("10",true));
	    limitButton.addItem(new CheckItem("20",false));
	    limitButton.addItem(new CheckItem("50",false));
	    limitButton.addItem(new CheckItem("100",false));
	    limitButton.addItem(new CheckItem("500",false));

	    /**
	     * The Layout. A border panel encloses the whole application
	     */
	    Panel panel = new Panel();  
	    panel.setBorder(false);  
	    panel.setPaddings(10);  
	    panel.setLayout(new FitLayout());
	    Panel borderPanel = new Panel();
	    borderPanel.setBorder(false);
	    borderPanel.setLayout(new BorderLayout());
	    
	    /**
	     * The north panel is used to display the title and other future menus
	     */
	    Panel northPanel = new Panel("", "<p>Teardrop, The personnal meta-search engine</p>");
	    northPanel.setHeight(32);
	    northPanel.setBodyBorder(false);
	    northPanel.setHeader(false);
	    borderPanel.add(northPanel, new BorderLayoutData(RegionPosition.NORTH));
	    
	    /**
	     * The west panel is used for the search parameters
	     */
	    Panel westPanel = new Panel();
	    westPanel.setLayout(new VerticalLayout(5));
	    westPanel.setTitle("Search Criterions");  
	    westPanel.setCollapsible(true);  
	    westPanel.setWidth(200);
	    westPanel.add(queryText);
	    westPanel.add(limitButton);
	    westPanel.add(searchButton);
	    westPanel.add(engTree);
	    westPanel.add(progressPanel);
	    borderPanel.add(westPanel,new BorderLayoutData(RegionPosition.WEST));
	    
	    /**
	     * The center panel contains the search results
	     */
	    Panel defaultCenterPanel = new Panel("Teardrop", "Welcome to Teardrop");
	    defaultCenterPanel.setLayout(new FitLayout());
	    centerPanel.add(defaultCenterPanel);
	    centerPanel.setDeferredRender(false);
	    centerPanel.setEnableTabScroll(true);
	    borderPanel.add(centerPanel, new BorderLayoutData(RegionPosition.CENTER));
	    
	    /**
	     * Render the objects
	     */
	    panel.add(borderPanel);
	    new Viewport(panel);
	  }
}
