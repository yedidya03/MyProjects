package GUI;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Panel;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

import Improved_MP.MP_DrivePath;
import Improved_MP.MP_Transition;
import drawables.TextCanvas;

public class BlocksPanel extends Panel{
	
	private Image 		i;
	private Graphics 	doubleG;

	
	private TrajectoryEditor trajectoryEditor;
	
	private ArrayList<TextCanvas> texts;
	
	public BlocksPanel() {
		setSize(250, SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL);
		setPreferredSize(new Dimension(250, SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL));
		
		setLayout(new FlowLayout(FlowLayout.LEADING, 10, 5));
	
		setBackground(SimulatorConstants.CONTROL_PANEL_BACKGROWND_COLOR);
	}
	
	public synchronized void setTrajectoryEditor(TrajectoryEditor traj) {
		trajectoryEditor = traj;
		
		removeAll();

		texts = new ArrayList<TextCanvas>();
		ArrayList<MP_DrivePath> tempPathArray = trajectoryEditor.getPathArray();
		
		TextCanvas textTemp;
		MP_DrivePath pathTemp;
		
		for (int i = 0; i < tempPathArray.size(); i ++) {
			pathTemp = tempPathArray.get(i);
			
			textTemp = new TextCanvas(pathTemp.toString(), getWidth() - 20);
			texts.add(textTemp);
			add(textTemp);
		}
		
		texts.get(0).setBackgrowndColor(SimulatorConstants.CHOSEN_COLOR);
		
		revalidate();
		
	}
	
	int counter = -1;
	
	public synchronized void updatePannel() {
		if (counter != trajectoryEditor.getCounter()){
			counter = trajectoryEditor.getCounter();
			
			for (int i = 0; i < texts.size(); i ++) {
				if (i != counter) {
					texts.get(i).setBackgrowndColor(SimulatorConstants.REGULAR_COLOR);
				}
			}
			texts.get(counter).setBackgrowndColor(SimulatorConstants.CHOSEN_COLOR);	
		}
	}
	
	@Override
	public void update(Graphics g) {
		if (i == null) {
			i = createImage(this.getSize().width, this.getSize().height);
			doubleG = i.getGraphics();
		}
		
		doubleG.setColor(getBackground());
		doubleG.fillRect(0, 0, this.getSize().width, this.getSize().height);
		
		doubleG.setColor(getForeground());
		paint(doubleG);
				
		g.drawImage(i, 0, 0, this);
	}
}
