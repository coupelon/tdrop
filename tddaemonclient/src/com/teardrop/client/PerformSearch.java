/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import com.google.gwt.http.client.Header;
import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.core.SortDir;
import com.gwtext.client.data.FieldDef;
import com.gwtext.client.data.IntegerFieldDef;
import com.gwtext.client.data.JsonReader;
import com.gwtext.client.data.Node;
import com.gwtext.client.data.Record;
import com.gwtext.client.data.RecordDef;
import com.gwtext.client.data.Store;
import com.gwtext.client.data.StringFieldDef;
import com.gwtext.client.util.Format;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.ProgressBar;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Toolbar;
import com.gwtext.client.widgets.ToolbarButton;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.grid.CellMetadata;
import com.gwtext.client.widgets.grid.ColumnConfig;
import com.gwtext.client.widgets.grid.ColumnModel;
import com.gwtext.client.widgets.grid.GridPanel;
import com.gwtext.client.widgets.grid.GridView;
import com.gwtext.client.widgets.grid.Renderer;
import com.gwtext.client.widgets.grid.RowParams;
import com.gwtext.client.widgets.grid.RowSelectionModel;
import com.gwtext.client.widgets.layout.FitLayout;
import com.gwtext.client.widgets.tree.TreeNode;
import com.gwtext.client.widgets.Window;

public class PerformSearch {
	private static final String DEFAULT_SEARCH_URL = "/services/query_post";
	private static final String DEFAULT_NEXT_URL = "/services/get_next_results";
	private static final String EXPORT_CSV_URL = "/services/export_csv?";
	EngineTree engTree;
	TextField queryText;
	CycleButton limitButton;
	TabPanel centerPanel;
	GridPanel resultsPanel;
	ProgressBar pBar;
	boolean showDetails = true;
	boolean showHighlight = true;
	Store resultsStore;
	Store enginesStore;
	String checkedNodeString;
	String tdsession = "";
	float numberOfEngines = 0;
	
	private final RecordDef recordResultsDef = new RecordDef(new FieldDef[]{
			new IntegerFieldDef("num"),
			new StringFieldDef("url"),  
			new StringFieldDef("title"),  
			new StringFieldDef("abstract"),  
			new StringFieldDef("img"),  
			new StringFieldDef("engines")
		});
	private final RecordDef recordEnginesDef = new RecordDef(new FieldDef[]{
			new IntegerFieldDef("cpt"),
			new StringFieldDef("name"),  
		});
	
	public PerformSearch(EngineTree engTree, TextField queryText, CycleButton limitButton, TabPanel centerPanel) {
		this.engTree = engTree;
		this.queryText = queryText;
		this.limitButton = limitButton;
		this.centerPanel = centerPanel;
		
//		String checkedNodeString = "";
//		TreeNode[] checkedNode = engTree.getChecked();
//		for(int i = 0; i < checkedNode.length; ++i) {
//			if (checkedNode[i].getAttribute("name") != null) {
//				checkedNodeString += (checkedNodeString.equals("")?"":",") + checkedNode[i].getAttribute("name");
//			}
//		}
		
		checkedNodeString = getChecked();
		
		if (queryText.getText().equals("") || checkedNodeString.equals("")) {
			return;
		}
		
		//Workaround to a bug:
		String limit = limitButton.getText().substring(limitButton.getPrependText().length());
		//String limit = limitButton.getActiveItem().getText()
		
		
		ColumnConfig numColumn = new ColumnConfig("#", "num",16);
		numColumn.setHidden(true);
		numColumn.setSortable(true);
		
		ColumnConfig titleColumn = new ColumnConfig("Title", "title", 600);  
		titleColumn.setCss("white-space:normal;");  
		titleColumn.setRenderer(renderTitle);
		titleColumn.setSortable(true);
		
		ColumnConfig engColumn = new ColumnConfig("Engines", "engines");
		engColumn.setRenderer(renderDefault);
		
		ColumnConfig imgColumn = new ColumnConfig("Images", "img");  
		imgColumn.setHidden(true);
		imgColumn.setRenderer(renderDefault);
		
		ColumnConfig abstractColumn = new ColumnConfig("Abstract", "abstract");  
		abstractColumn.setHidden(true);
		abstractColumn.setRenderer(renderDefault);
		
		ColumnConfig urlColumn = new ColumnConfig("Url", "url");
		urlColumn.setHidden(true);
		urlColumn.setRenderer(renderDefault);
		
		ColumnModel columnModel = new ColumnModel(new ColumnConfig[]{
				numColumn,
				titleColumn,
				engColumn,
				abstractColumn,  
				urlColumn,
				imgColumn
		});
		
		columnModel.setDefaultSortable(true);

		GridView view = new GridView() {  
			public String getRowClass(Record record, int index, RowParams rowParams, Store store) {
				if (showDetails) {
					String img = getDecodedRecord(record,"img");
					if (!img.equals("")) {
						rowParams.setBody("<img width=\"40\" height =\"30\" src=\"" + img + "\" />" + getDecodedRecord(record,"abstract"));
					} else {
						rowParams.setBody(getHighLight(getDecodedRecord(record,"abstract")));
					}
					rowParams.setBodyStyle("text-align:middle;");
					return "x-grid3-row-expanded";  
				} else {  
					return "x-grid3-row-collapsed";  
				}  
			}  
		};  

		view.setForceFit(true);  
		view.setAutoFill(true);
		view.setEnableRowBody(true);
		
		JsonReader resultsReader = new JsonReader(recordResultsDef);
		resultsReader.setRoot("results");
		resultsStore = new Store(resultsReader);
		resultsStore.setDefaultSort("num", SortDir.ASC);
		
		JsonReader enginesReader = new JsonReader(recordEnginesDef);
		enginesReader.setRoot("engines");
		enginesStore = new Store(enginesReader);
		
		resultsPanel = new GridPanel();
		resultsPanel.setTitle(URL.encodeComponent(queryText.getText()));
		resultsPanel.setTrackMouseOver(true);
		resultsPanel.setSelectionModel(new RowSelectionModel());
		resultsPanel.setView(view);
		resultsPanel.setStore(resultsStore);
		resultsPanel.setColumnModel(columnModel);
		resultsPanel.setClosable(true);
		resultsPanel.setAutoExpandColumn("Title");
		resultsPanel.setStripeRows(true);
		
		Toolbar toolbar = new Toolbar();  
		ToolbarButton moreInfo = new ToolbarButton("More Information", new ButtonListenerAdapter() {  
			public void onClick(final Button innerButton, EventObject e) {
				Window infoWindow = new Window("Search Information", true, true);
				infoWindow.setClosable(true);
				infoWindow.setMinWidth(250);
				infoWindow.setMinHeight(175);
				infoWindow.setHeight(200);
				infoWindow.setPaddings(10);
				infoWindow.setLayout(new FitLayout());
				String html = "<b>Results number</b>: " + resultsStore.getCount() + "<br /><br />";

				for (int i = 0; i < enginesStore.getCount(); ++i) {
					if (enginesStore.getAt(i).getAsInteger("cpt") != -1) {
						html += "<b>" + enginesStore.getAt(i).getAsString("name") +
								"</b>: " + enginesStore.getAt(i).getAsInteger("cpt") + "<br />";
					}
				}
				
				Panel engPanel = new Panel("", html);
				engPanel.setBodyBorder(false);
				engPanel.setHeader(false);
				engPanel.setBaseCls("x-plain");
				engPanel.setAutoScroll(true);
				infoWindow.add(engPanel);
				
				infoWindow.show();
			}  
		});
		moreInfo.setCls("x-btn-text-icon info");
		moreInfo.setTooltip("Get detailed information about this search.");
		
		ToolbarButton exportCsv = new ToolbarButton("Export as CSV");
		exportCsv.setCls("x-btn-text-icon csv");  
		exportCsv.addListener(new ButtonListenerAdapter() {  
			public void onClick(Button button, EventObject e) {  
				com.google.gwt.user.client.Window.open(EXPORT_CSV_URL + tdsession, "_blank", "menubar=no,location=no,resizable=no,scrollbars=no,status=no");
			}
		});
		exportCsv.setTooltip("Export the results in a spreadsheet.");
		
		ToolbarButton toggleDetails = new ToolbarButton("Detailed");  
		toggleDetails.setPressed(showDetails);
		toggleDetails.setEnableToggle(true);  
		toggleDetails.setCls("x-btn-text-icon details");  
		toggleDetails.addListener(new ButtonListenerAdapter() {  
			public void onToggle(Button button, boolean pressed) {  
				showDetails = pressed;  
				resultsPanel.getView().refresh();  
			}  
		});
		toggleDetails.setTooltip("Toggle between simple and detailed view");
		
		ToolbarButton toggleHighlight = new ToolbarButton("Highlighting");
		toggleHighlight.setPressed(showHighlight);
		toggleHighlight.setEnableToggle(true);
		toggleHighlight.setCls("x-btn-text-icon highlight");  
		toggleHighlight.addListener(new ButtonListenerAdapter() {  
			public void onToggle(Button button, boolean pressed) {  
				showHighlight = pressed;  
				resultsPanel.getView().refresh();  
			}  
		});
		toggleHighlight.setTooltip("Highlight the search keywords");
		
		pBar = new ProgressBar();
		pBar.setWidth(300);
		pBar.setText("Searching...");
		
		toolbar.addFill();
		toolbar.addButton(exportCsv);
		toolbar.addButton(moreInfo);
		toolbar.addSeparator();
		toolbar.addButton(toggleDetails);
		toolbar.addButton(toggleHighlight);
		toolbar.addSeparator();
		toolbar.addElement(pBar.getElement());
		resultsPanel.setBottomToolbar(toolbar);
		
		centerPanel.add(resultsPanel);
		centerPanel.activate(centerPanel.getItems().length-1);
		
		doPostURL("query=" + queryText.getText() + ";engines=" + checkedNodeString + ";limit=" + limit,DEFAULT_SEARCH_URL);
		
    }
	
	/*
	 * Fetch the requested URL.
	 */
	private void doPostURL(String post, String url) {
	    RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(url));
	    try {
	    	builder.setHeader("Content-Length", String.valueOf(post.length()));
	    	if (!tdsession.equals("")) {
	    		builder.setHeader("TDSession", tdsession);
	    	}
	    	builder.sendRequest(post, new RequestCallback() {
	    	    public void onError(Request request, Throwable exception) {
	    	    	setProgessMessage("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
	    	    }

	    	    public void onResponseReceived(Request request, Response response) {
	    	      if (200 == response.getStatusCode()) {
	    	    	  parseCookies(response);
	    	    	  updateGrid(response.getText());
	    	      } else {
	    	    	  setProgessMessage("Incorrect status: " + response.getStatusText());
	    	      }
	    	    }
	    	  });
	    	} catch (RequestException e) {
	    		setProgessMessage("Couldn't connect to server (could be timeout, SOP violation, etc.)");      
	    	}
	  }
	
	private void parseCookies(final Response pResponse) {
        final Header[] headers = pResponse.getHeaders();
        if (headers == null || headers.length == 0) {
        	return;
        }
        for (int i = 0; i < headers.length; i++) {
            if (headers[i] != null && "Set-TDSession".equalsIgnoreCase(headers[i].getName())) {
            	tdsession = headers[i].getValue();
            }
        }
	}
	
	private Renderer renderTitle = new Renderer() {  
		public String render(Object value, CellMetadata cellMetadata, Record record,  
							int rowIndex, int colNum, Store store) {
			
			return Format.format("<b><a href=\"{1}\"target=\"_blank\">{0}</a></b>",  
					new String[]{getHighLight(getDecodedRecord(record,"title")),  
					getDecodedRecord(record,"url")  
					});  
		}
	};
	
	private Renderer renderDefault = new Renderer() {
		public String render(Object value, CellMetadata cellMetadata, Record record,  
							int rowIndex, int colNum, Store store) {  
			if (value == null) return ""; 
			return URL.decodeComponent((String) value);
		}
	};
	
	private String getDecodedRecord(Record r,String name) {
		if (r.getAsString(name) == null) return "";
		return URL.decodeComponent(r.getAsString(name));
	}
	
	//This method highlights the query keywords in the given string.
	//Instead of using replaceAll(), I used that to make it portably
	// case-insensitive (Not well supported by GWT otherwise).
	private String getHighLight(String s) {
		if (!showHighlight) return s;
		String hlighted = s;
		String[] keys = queryText.getText().toLowerCase().split(" ");
		for(int i = 0; i < keys.length; ++i) {
			String sLower = hlighted.toLowerCase();
			int mindex = sLower.length();
			while(mindex > 1) {
				mindex = sLower.lastIndexOf(keys[i], mindex-1);
				if (mindex != -1) {
					hlighted = hlighted.substring(0, mindex) + "<span class=\"hl\">" + hlighted.substring(mindex,mindex+keys[i].length()) + "</span>" + hlighted.substring(mindex+keys[i].length());
				}
			}
		}
		return hlighted;
	}
	
	private void updateGrid(String jsonString) {
		if (jsonString.startsWith("{\"final\":\"false\"")) {
			doPostURL("",DEFAULT_NEXT_URL);
		}
		try  {
		resultsStore.loadJsonData(jsonString, false);
		enginesStore.loadJsonData(jsonString, false);
		resultsPanel.getView().refresh();
		} catch (Exception e) {
			setProgessMessage("updateGrid: an error was catched: " + e.getMessage());
		}
		updateProgress();
	}
	
	private void updateProgress() {
		int count = 0;
		float results = 0;
		for (int i = 0; i < enginesStore.getCount(); ++i) {
			if (enginesStore.getAt(i).getAsInteger("cpt") != -1) {
				count++;
				results += enginesStore.getAt(i).getAsInteger("cpt");
			}
		}
		setProgessMessage(count/numberOfEngines, resultsStore.getCount() + " (" + results + ") Results (" + count + "/" + numberOfEngines + ")");
	}
	
	//To circumvent the tree checks bug...
	public String getChecked() {
		String checkedNodeString = "";
		TreeNode n = engTree.getRootNode();
		Node[] tnChild = n.getChildNodes();
		for (int i = 0; i < tnChild.length; ++i) {
			Node[] tnEng = tnChild[i].getChildNodes();
			for (int j=0; j < tnEng.length; ++j) {
				if (((TreeNode) tnEng[j]).getUI().isChecked() || ((TreeNode) tnChild[i]).getUI().isChecked()) {
					checkedNodeString += (checkedNodeString.equals("")?"":",") + tnEng[j].getAttribute("name");
					numberOfEngines++;
				}
			}
		}
		return checkedNodeString;
	}
	
	private void setProgessMessage(float value, String text) {
		pBar.updateProgress(value,text);
	}
	
	private void setProgessMessage(String text) {
		pBar.updateProgress(0,text);
	}
}
