package GUI;

import java.awt.Button;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Panel;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

import MP_ImprovedCommands.MP_AutoGenerator;
import drawables.TextCanvas;
import filesHandling.TrajectoryFile;

public class FilesControlPanel extends Panel{

	private ArrayList<TextCanvas> texts = new ArrayList<TextCanvas>();
	private TextCanvas hardCodedText;
	
	private File directory = new File(SimulatorConstants.FILES_DIRECTORY);
	private TrajectoryFile[] trajectoryFiles;
	
	private TrajectoryEditor hardCodedTraj;
		
	private FieldCanvas field;
	private BlocksPanel blocks;
	
	public FilesControlPanel() {
		setSize(250, SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL);
		setPreferredSize(new Dimension(250, SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL));

		setLayout(new FlowLayout(FlowLayout.CENTER, 10, 5));
		setBackground(SimulatorConstants.CONTROL_PANEL_BACKGROWND_COLOR);
		
		
		try {
			hardCodedTraj = new TrajectoryEditor();

			hardCodedTraj.setTrajectory(new HardCodedTrajectory());
			hardCodedTraj.setStartingPoint(HardCodedTrajectory.startingPoint, HardCodedTrajectory.angle);
		} catch (Exception e) {
			hardCodedTraj = null;
			e.printStackTrace();
		}
		
	}
	
	public void init(FieldCanvas field, BlocksPanel blocks) {
		this.field = field;
		this.blocks = blocks;
		
		if (hardCodedTraj != null) {
			hardCodedText = new TextCanvas("Hard Coded Path", getWidth() - 20);
			hardCodedText.addMouseListener(new FileTextsMouseListener(0));
			texts.add(hardCodedText);
			add(hardCodedText);
		}
		
		TextCanvas temp;
		File[] files = directory.listFiles(new FilenameFilter() { 
                 public boolean accept(File dir, String filename)
                      { return filename.endsWith(".txt"); }
        } );
		trajectoryFiles = new TrajectoryFile[files.length];
		for (int i = 0; i < files.length; i++ ) {
			trajectoryFiles[i] = new TrajectoryFile(files[i]);
			temp = new TextCanvas(files[i].getName().substring(0, files[i].getName().length() - 4), getWidth() - 20);
			temp.addMouseListener(new FileTextsMouseListener(texts.size()));
			add(temp);
			texts.add(temp);
		}
		
		texts.get(0).setBackgrowndColor(SimulatorConstants.CHOSEN_COLOR);		
		
		/*
		Panel p1 = new Panel();
		p1.setLayout(null);
		p1.setBackground(Color.GREEN);
		p1.setSize(200, 400);
		p1.setPreferredSize(new Dimension(200, 200));
		Button b1 = new Button("btsf sd" );
		b1.setBounds(100, 100, 50, 20);
		p1.add(b1);
		add(p1);
		*/
		
		try {
			if (hardCodedTraj == null) {
				TrajectoryEditor traj = trajectoryFiles[0].readFile();
				field.setTrajectoryEditor(traj);
				blocks.setTrajectoryEditor(traj);
			} else {
				field.setTrajectoryEditor(hardCodedTraj);
				blocks.setTrajectoryEditor(hardCodedTraj);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	private class FileTextsMouseListener implements MouseListener {

		private int index;
		
		public FileTextsMouseListener(int i) {
			this.index = i;
		}
		
		@Override
		public void mouseClicked(MouseEvent e) {
			
		}

		@Override
		public void mouseEntered(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseExited(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mousePressed(MouseEvent e) {
			for (int i = 0; i < texts.size(); i ++) {
				if (i != index) {
					texts.get(i).setBackgrowndColor(SimulatorConstants.REGULAR_COLOR);
				}
			}
			
			texts.get(index).setBackgrowndColor(SimulatorConstants.CHOSEN_COLOR);	
			
			int indexSub = hardCodedTraj == null? 0 : 1;
			
			if (index == 0 && indexSub > 0) {
				try {	
					field.setTrajectoryEditor(hardCodedTraj);
					blocks.setTrajectoryEditor(hardCodedTraj);
				} catch (Exception e1) {
					e1.printStackTrace();
				}
				
				return;
			}
			
			try {
				TrajectoryEditor temp = trajectoryFiles[index - indexSub].readFile();
				field.setTrajectoryEditor(temp);
				blocks.setTrajectoryEditor(temp);
				//temp.getTrajectory().printVel(true);
				//temp.getTrajectory().printVel(false);
			} catch (Exception e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}

		@Override
		public void mouseReleased(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}
	}
	
	private static class HardCodedTrajectory extends MP_AutoGenerator {
		
		public static DoublePoint startingPoint;
		public static double angle;
		
		public HardCodedTrajectory() throws Exception {

			startingPoint = new DoublePoint(1.1, SimulatorConstants.ROBOT_HEIGHT_METER / 2);
			
			addStraight(5, 0);
			
			/*addStraight(4, 0.5);
			addRadius(1.2, 90, 0.5, 0.5, 0.5, 1, 1);
			addStraight(3.1, 0.5);
			addRadius(1.2, 90, 0.5, 0.5, 0.5, 1, 1);
			addStraight(2, 0);
			 */
			
			
			
			/*
			//rr
			addStraight(4, 2);
			addRadius(1.2, 90, 2, 2, 1.5, 3);
			addStraight(2.8, 3.5, 1.5, 2, 4);
			addRadius(-0.8, 115, 1.5, 0, 1.5, 3);
			
			addRadius(-1.3, -60, 2, 0, 1.5, 3);
			addRadius(-1.3, 60, 2, 0, 1.5, 3);
			
			addRadius(-1, -75, 1.3, 1.3, 1.5, 3);
			addStraight(-0.6, 0);
			
			addStraight(0.6, 1.3);
			addRadius(-1,  75, 1.3, 0, 1.5, 3);
			*/
						
			/*
			//ll
			addStraight(1.45, 2);
			addRadius(20, 14, 3, 0, 2);
			
			addRadius(1.2, -45, 0);
			addStraight(-0.45, 0);
			
			
			addStraight(0.25, 1);
			
			addRadius(1.2, 45, 0);
			
			
			addRadius(0.8, -78, 0);
			addStraight(-0.85, 0);
			
			addStraight(0.45, 1.1);
			addRadius(1, 87, 0);
			*/
			
			/*
			// middle
			startingPoint = new DoublePoint(4.25,
					SimulatorConstants.ROBOT_HEIGHT_METER / 2);
			
			double leftOrRigth = -1;
			
			addRadius(leftOrRigth * -1.2, 60, 1.3, 1.2, 1.2);
			addRadius(leftOrRigth * 1.2, 60, 1.3, 1, 1.2);
			addStraight(0.3, 1.5, 0, 2);
			
			addStraight(-0.3, 1.5, 1, 2);
			addRadius(leftOrRigth * 1.2, -60, 1.3, 0, 1.2);
			
			addRadius(leftOrRigth * 0.001, 90, 1.5, 0, 2);
			addStraight(0.45, 0);
			
			addStraight(-0.45, 0);
			addRadius(leftOrRigth * 0.001, -90, 1.5, 0, 2);
			addRadius(leftOrRigth * 1.2, 60, 1.3, 1, 1.2);
			addStraight(0.3, 1.5, 0, 2);
			*/
			
			/*
			//--------------------------------//
			//------------RL_Switch-----------//
			addStraight(4, 2);//3.65
			addRadius(1.2, 90, 2, 2, 1.5);
			addStraight(2, 3.5, 0.5, 1.5);
			addRadius(0.67/2, 90, 2, 0, 2);
			//ag.addStraight(0.2, 0);
			
			addRadius(1.1, -30, 2, 0, 1.5);
			addStraight(0.5, 0);
			
			addRadius(0.67/2, -60, 0);
			addStraight(-2.3, 3, 1.5, 1.5);
			addRadius(-0.8, -115, 1.5, 0, 1.5);
			//--------------------------------//
			//--------------------------------//
			*/
			
			/*
			//--------------------------------//
			//------------LR_Switch-----------//
			addStraight(1.45, 2);
			addRadius(20, 15, 3, 0, 2);
			
			addRadius(1.2, -45, 0);
			addStraight(-0.5, 0);
			
			// put cube 1
			
			addStraight(0.5, 0);
			
			//collect cube 2
			addStraight(-0.5, 0);
			
			addRadius(0.1, -90, 0);
			addRadius(-0.8, -30, 0.7);
			addStraight(-2.5, 2, 1.3, 1.5);
			
			addRadius(0.8, -115, 1.5, 0, 1.5);
			//--------------------------------//
			//--------------------------------//
			*/
			generateAuto();
		}
	}
}
