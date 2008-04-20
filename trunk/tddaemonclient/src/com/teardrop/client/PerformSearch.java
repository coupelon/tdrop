package com.teardrop.client;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.TextBox;
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
import com.gwtext.client.widgets.MessageBox;
import com.gwtext.client.widgets.MessageBoxConfig;
import com.gwtext.client.widgets.Panel;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Toolbar;
import com.gwtext.client.widgets.ToolbarButton;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.grid.CellMetadata;
import com.gwtext.client.widgets.grid.ColumnConfig;
import com.gwtext.client.widgets.grid.ColumnModel;
import com.gwtext.client.widgets.grid.GridPanel;
import com.gwtext.client.widgets.grid.GridView;
import com.gwtext.client.widgets.grid.Renderer;
import com.gwtext.client.widgets.grid.RowParams;
import com.gwtext.client.widgets.grid.RowSelectionModel;
import com.gwtext.client.widgets.tree.TreeNode;

public class PerformSearch {
	private static final String DEFAULT_SEARCH_URL = "/services/query_post";
	private static final String DEFAULT_NEXT_URL = "/services/get_next_results";
	EngineTree engTree;
	TextBox queryText;
	CycleButton limitButton;
	TabPanel centerPanel;
	GridPanel resultsPanel;
	Panel progressPanel;
	boolean showDetails = true;
	Store resultsStore;
	Store enginesStore;
	String checkedNodeString;
	
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
	
	public PerformSearch(EngineTree engTree, TextBox queryText, CycleButton limitButton, TabPanel centerPanel, Panel progressPanel) {
		this.engTree = engTree;
		this.queryText = queryText;
		this.limitButton = limitButton;
		this.centerPanel = centerPanel;
		this.progressPanel = progressPanel;
		
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
						rowParams.setBody(getDecodedRecord(record,"abstract"));
					}
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
				MessageBox.show(new MessageBoxConfig() {  
					{  
						setTitle("Search Information");  
						setMsg("<b>Results number</b>: " + resultsStore.getCount() + "<br />" +
							   "<b>Engines:</b>: " + checkedNodeString);  
						setWidth(300);  
						setButtons(MessageBox.OK);
						setAnimEl(innerButton.getId());  
					}  
				});
			}  
		});
		moreInfo.setCls("x-btn-text-icon info");
		moreInfo.setTooltip("Get detailed information about this search.");
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
		
		toolbar.addFill();
		toolbar.addButton(moreInfo);
		toolbar.addButton(toggleDetails);
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
	    	builder.sendRequest(post, new RequestCallback() {
	    	    public void onError(Request request, Throwable exception) {
	    	    	Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
	    	    }

	    	    public void onResponseReceived(Request request, Response response) {
	    	      if (200 == response.getStatusCode()) {
	    	    	  updateGrid(response.getText());
	    	      } else {
	    	    	  Window.alert("Incorrect status: " + response.getStatusText());
	    	      }
	    	    }
	    	  });
	    	} catch (RequestException e) {
	    		Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");      
	    	}
	  }
	
	private Renderer renderTitle = new Renderer() {  
		public String render(Object value, CellMetadata cellMetadata, Record record,  
							int rowIndex, int colNum, Store store) {
			
			return Format.format("<b><a href=\"{1}\"target=\"_blank\">{0}</a></b>",  
					new String[]{getDecodedRecord(record,"title"),  
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
	
	private void updateGrid(String jsonString) {
		//Window.alert(jsonString);
		if (jsonString.startsWith("{\"final\":\"false\"")) {
			doPostURL("",DEFAULT_NEXT_URL);
		}
		try  {
		resultsStore.loadJsonData(jsonString, false);
		enginesStore.loadJsonData(jsonString, false);
		resultsPanel.getView().refresh();
		} catch (Exception e) {
			Window.alert("updateGrid: an error was catched: " + e.getMessage());
		}
		for (int i = 0; i < enginesStore.getCount(); ++i) {
			//Window.alert(enginesStore.getAt(i).getAsString("name"));
		}
	}
	
	//To circumvent the three checks bug...
	public String getChecked() {
		String checkedNodeString = "";
		TreeNode n = engTree.getRootNode();
		Node[] tnChild = n.getChildNodes();
		for (int i = 0; i < tnChild.length; ++i) {
			Node[] tnEng = tnChild[i].getChildNodes();
			for (int j=0; j < tnEng.length; ++j) {
				if (((TreeNode) tnEng[j]).getUI().isChecked() || ((TreeNode) tnChild[i]).getUI().isChecked())
					checkedNodeString += (checkedNodeString.equals("")?"":",") + tnEng[j].getAttribute("name");
			}
		}
		return checkedNodeString;
	}
}
