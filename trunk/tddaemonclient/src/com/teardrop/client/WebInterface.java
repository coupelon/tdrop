/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import com.google.gwt.core.client.EntryPoint;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.core.RegionPosition;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Viewport;
import com.gwtext.client.widgets.form.FormPanel;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.layout.AnchorLayoutData;
import com.gwtext.client.widgets.layout.BorderLayout;
import com.gwtext.client.widgets.layout.BorderLayoutData;
import com.gwtext.client.widgets.layout.FitLayout;
import com.gwtext.client.widgets.layout.RowLayoutData;
import com.gwtext.client.widgets.menu.CheckItem;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class WebInterface implements EntryPoint {
	  private static final String SEARCH_BUTTON_DEFAULT_TEXT = "Search";
	  private TextField queryText = new TextField("");
	  private CycleButton limitButton = new CycleButton();
	  private TabPanel centerPanel = new TabPanel();
	  private EngineTree engTree = new EngineTree(centerPanel);
	  private Button searchButton = new Button(SEARCH_BUTTON_DEFAULT_TEXT);

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
	    borderPanel.setPaddings(5);
	    
	    /**
	     * The north panel is used to display the title and other future menus
	     */
	    Panel northPanel = new Panel("", "<div class='inside'><h2><img src='imgs/tdlogo.png' alt='Logo' />&nbsp;<a href='http://www.teardrop.fr/'>Teardrop</a></h2><p class='description'>The personal meta search engine</p></div>");
	    northPanel.setBodyBorder(false);
	    northPanel.setHeader(false);
	    northPanel.setId("header");
	    northPanel.setHeight(40);
	    borderPanel.add(northPanel, new BorderLayoutData(RegionPosition.NORTH));
	    
	    /**
	     * The panel for the search criterions
	     */
	    FormPanel criterPanel = new FormPanel();
	    //criterPanel.setLayout(new RowLayout());
	    criterPanel.setPaddings(5);
	    criterPanel.setMargins(0, 0, 0, 10);
	    criterPanel.setTitle("Search Criterions");
	    criterPanel.setCollapsible(true);
	    
	    queryText.setHideLabel(true);
	    queryText.setTabIndex(0);
	    queryText.addKeyListener(EventObject.ENTER, new OnSearchSubmit(engTree, queryText,limitButton,centerPanel));
	    criterPanel.add(queryText, new AnchorLayoutData("100%"));
	    
	    limitButton.setShowText(true);  
	    limitButton.setPrependText("Results per engines: ");
	    limitButton.addItem(new CheckItem("10",true));
	    limitButton.addItem(new CheckItem("20",false));
	    limitButton.addItem(new CheckItem("50",false));
	    limitButton.addItem(new CheckItem("100",false));
	    limitButton.addItem(new CheckItem("500",false));
	    limitButton.setTabIndex(1);
	    criterPanel.add(limitButton);
	    
	    searchButton.addListener(new OnSearchSubmit(engTree, queryText,limitButton,centerPanel));
	    searchButton.setTabIndex(2);
	    searchButton.setCls("x-btn-text-icon btn-search-icon");
	    searchButton.setId("search-button");
	    criterPanel.add(searchButton);
	    
	    /**
	     * The west panel is used for the search parameters
	     */
	    Panel westPanel = new Panel();
	    westPanel.setWidth(185);
	    westPanel.setBodyBorder(false);
	    westPanel.setPaddings(0, 0, 10, 0);
	    westPanel.add(criterPanel);
	    westPanel.add(engTree);
	    westPanel.setAutoScroll(true);
	    borderPanel.add(westPanel,new BorderLayoutData(RegionPosition.WEST));
	    
	    /**
	     * The center panel contains the search results
	     */
	    Panel defaultCenterPanel = new Panel("Teardrop", "<div class='inside'><p>Welcome to Teardrop</p></div>");
	    defaultCenterPanel.setLayout(new FitLayout());
	    centerPanel.add(defaultCenterPanel, new RowLayoutData("100%"));
	    centerPanel.setDeferredRender(false);
	    centerPanel.setEnableTabScroll(true);
	    centerPanel.setId("centerpanel");
	    borderPanel.add(centerPanel, new BorderLayoutData(RegionPosition.CENTER));
	    
	    /**
	     * Render the objects
	     */
	    panel.add(borderPanel);
	    new Viewport(panel);
	  }
}
