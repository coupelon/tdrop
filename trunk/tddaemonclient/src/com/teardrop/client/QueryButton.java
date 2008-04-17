package com.teardrop.client;

import java.util.Date;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.json.client.JSONArray;
import com.google.gwt.json.client.JSONParser;
import com.google.gwt.json.client.JSONValue;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.TextBox;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.core.SortDir;  
import com.gwtext.client.core.TextAlign;  
import com.gwtext.client.data.*;  
import com.gwtext.client.util.DateUtil;  
import com.gwtext.client.util.Format;  
import com.gwtext.client.widgets.*;  
import com.gwtext.client.widgets.event.ButtonListenerAdapter;  
import com.gwtext.client.widgets.event.PanelListenerAdapter;  
import com.gwtext.client.widgets.grid.*;
import com.gwtext.client.widgets.layout.RowLayout;
import com.gwtext.client.widgets.tree.TreeNode;

public class QueryButton extends Button {
	public QueryButton(String text,EngineTree engTree, TextBox queryText, CycleButton limitButton,TabPanel centerPanel) {
		super(text, new OnClickAdapter(engTree, queryText,limitButton,centerPanel));
	}
		
	private static class OnClickAdapter extends ButtonListenerAdapter {
		private static final String DEFAULT_SEARCH_URL = "/services/query_post";
		private static final String DEFAULT_NEXT_URL = "/services/get_next_results";
		EngineTree engTree;
		TextBox queryText;
		CycleButton limitButton;
		TabPanel centerPanel;
		GridPanel resultsPanel = null;
		boolean showPreview = true;
		Store store;
		JsonReader reader;
		public OnClickAdapter(EngineTree engTree, TextBox queryText, CycleButton limitButton, TabPanel centerPanel) {
			this.engTree = engTree;
			this.queryText = queryText;
			this.limitButton = limitButton;
			this.centerPanel = centerPanel;
		}
		
		//To circumvent the three checks bug...
		public String getChecked() {
			String checkedNodeString = "";
			TreeNode n = engTree.getRootNode();
			Node[] tnChild = n.getChildNodes();
			for (int i = 0; i < tnChild.length; ++i) {
				Node[] tnEng = tnChild[i].getChildNodes();
				for (int j=0; j < tnEng.length; ++j) {
					if (((TreeNode) tnEng[j]).getUI().isChecked())
						checkedNodeString += (checkedNodeString.equals("")?"":",") + tnEng[j].getAttribute("name");
				}
			}
			return checkedNodeString;
		}
		
		public void onClick(Button button, EventObject e) {
//			String checkedNodeString = "";
//			TreeNode[] checkedNode = engTree.getChecked();
//			for(int i = 0; i < checkedNode.length; ++i) {
//				if (checkedNode[i].getAttribute("name") != null) {
//					checkedNodeString += (checkedNodeString.equals("")?"":",") + checkedNode[i].getAttribute("name");
//				}
//			}
			String checkedNodeString = getChecked();
			
			//Workaround to a bug:
			String limit = limitButton.getText().substring(limitButton.getPrependText().length());
			//String limit = limitButton.getActiveItem().getText()
			
			ColumnConfig titleColumn = new ColumnConfig("Title", "title", 600);  
			titleColumn.setCss("white-space:normal;");  
			titleColumn.setRenderer(renderTitle);
			
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
					titleColumn,
					engColumn,
					imgColumn,  
					abstractColumn,  
					urlColumn  
			});
			
			columnModel.setDefaultSortable(true);

			GridView view = new GridView() {  
				public String getRowClass(Record record, int index, RowParams rowParams, Store store) {
					if (showPreview) {
//						String img = "";
//						if (!record.getAsString("img").equals("")) {
//							img = "<img src=\"" + URL.decodeComponent(record.getAsString("img")) + "\"/>";
//						}
//						rowParams.setBody(Format.format("{0}<br /><a href=\"{1}\">{1}</a></p>",
//											new String[]{URL.decodeComponent(record.getAsString("abstract")),  
//														 URL.decodeComponent(record.getAsString("url"))}));
						rowParams.setBody(URL.decodeComponent(record.getAsString("abstract")));
							return "x-grid3-row-expanded";  
					} else {  
							return "x-grid3-row-collapsed";  
					}  
				}  
			};  

			view.setForceFit(true);  
			view.setAutoFill(true);
			view.setEnableRowBody(true);
			
			final RecordDef recordDef = new RecordDef(new FieldDef[]{  
					new StringFieldDef("url"),  
					new StringFieldDef("title"),  
					new StringFieldDef("abstract"),  
					new StringFieldDef("img"),  
					new StringFieldDef("engines")
				});
			reader = new JsonReader(recordDef);
			store = new Store(reader);
			store.setDefaultSort("title", SortDir.DESC); 
			
			resultsPanel = new GridPanel();
			resultsPanel.setTitle(URL.encodeComponent(queryText.getText()));
			resultsPanel.setAutoWidth(true);
			resultsPanel.setAutoHeight(true);
			resultsPanel.setAutoScroll(true);
			resultsPanel.setTrackMouseOver(true);  
			resultsPanel.setLoadMask(false);
			resultsPanel.setSelectionModel(new RowSelectionModel());  
			resultsPanel.setFrame(false);
			resultsPanel.setStripeRows(true);  
			//resultsPanel.setIconCls("grid-icon");
			resultsPanel.setView(view);
			resultsPanel.setStore(store);
			resultsPanel.setColumnModel(columnModel);
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
		    	    	  Window.alert(response.getStatusText());
		    	      }
		    	    }
		    	  });
		    	} catch (RequestException e) {
		    	  // Couldn't connect to server        
		    	}
		  }
		
		private Renderer renderTitle = new Renderer() {  
			public String render(Object value, CellMetadata cellMetadata, Record record,  
								int rowIndex, int colNum, Store store) {  
				return Format.format("<b><a href=\"{1}\"target=\"_blank\">{0}</a></b>",  
						new String[]{URL.decodeComponent((String) value),  
								URL.decodeComponent(record.getAsString("url"))  
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
		
		private void updateGrid(String jsonString) {			
			JSONValue jsonValue = JSONParser.parse(jsonString);
			if (JSONFunctions.getJSONSet(jsonValue,"preresults") != null) {
				reader.setRoot("preresults");
				doPostURL("",DEFAULT_NEXT_URL);
			} else {
				if (JSONFunctions.getJSONSet(jsonValue,"results") != null) {
					reader.setRoot("results");
				} else return;
			}
			
			store.loadJsonData(jsonString, true);
			resultsPanel.getView().refresh();
		}
	}
}
