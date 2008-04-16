package com.teardrop.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.RegionPosition;
import com.gwtext.client.widgets.BoxComponent;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Viewport;
import com.gwtext.client.widgets.layout.BorderLayout;
import com.gwtext.client.widgets.layout.BorderLayoutData;
import com.gwtext.client.widgets.layout.FitLayout;
import com.gwtext.client.widgets.layout.RowLayout;
import com.gwtext.client.widgets.menu.CheckItem;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";
	  private EngineTree engTree = new EngineTree();
	  private TextBox queryText = new TextBox();
	  private CycleButton limitButton = new CycleButton();
	  final Panel resultsPanel = new Panel();
	  private QueryButton searchButton = new QueryButton(SEARCH_BUTTON_DEFAULT_TEXT,engTree,queryText,limitButton,resultsPanel);

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

	    
	    resultsPanel.setLayout(new RowLayout());
	    resultsPanel.setAutoScroll(true);
	    
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
	    panel.setPaddings(15);  
	    panel.setLayout(new FitLayout());
	    Panel borderPanel = new Panel();  
	    borderPanel.setLayout(new BorderLayout());
	    
	    /**
	     * The north panel is used to display the title and other future menus
	     */
	    BoxComponent northPanel = new BoxComponent();  
	    northPanel.setEl(new HTML("<p>Teardrop, The personnal meta-search engine</p>").getElement());  
	    northPanel.setHeight(32);  
	    borderPanel.add(northPanel, new BorderLayoutData(RegionPosition.NORTH));
	    
	    /**
	     * The west panel is used for the search parameters
	     */
	    Panel westPanel = new Panel();  
	    westPanel.setTitle("Search Criterions");  
	    westPanel.setCollapsible(true);  
	    westPanel.setWidth(200);
	    westPanel.add(queryText);
	    westPanel.add(limitButton);
	    westPanel.add(searchButton);
	    westPanel.add(engTree);
	    borderPanel.add(westPanel,new BorderLayoutData(RegionPosition.WEST));
	    
	    /**
	     * The center panel contains the search results
	     */
	    
	    TabPanel centerPanel = new TabPanel();  
	    centerPanel.setDeferredRender(false);  
	    centerPanel.setActiveTab(0);
	    centerPanel.add(resultsPanel);
	    borderPanel.add(centerPanel, new BorderLayoutData(RegionPosition.CENTER));
	    
	    /**
	     * Render the objects
	     */
	    panel.add(borderPanel);
	    new Viewport(panel);
	  }
}
