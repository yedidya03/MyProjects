package GUI;

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.ArrayList;

import Improved_MP.MP_DrivePath;
import MP_ImprovedCommands.MP_AutoGenerator;
import drawables.Drawable;
import drawables.RobotDraw;
import drawables.TimerView;

public class FieldCanvas extends Canvas implements KeyListener, MouseListener, MouseWheelListener{

	private Image 		i = null, 
						backgrownd = null;
	private Graphics 	doubleG;
	
	private boolean 	pause = true;
	
	private RobotDraw 	robot;
	private TimerView	timerView;

	
	private ArrayList<Drawable> drawings = new ArrayList<Drawable>();
	
	private TrajectoryEditor trajectory;
	
	public FieldCanvas() {
		setSize(SimulatorConstants.FIELD_PICTIRUE_WIDTH_PIXEL,
				SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL);
		
		addKeyListener(this);
		addMouseListener(this);
		addMouseWheelListener(this);
		
		robot = new RobotDraw(
				new DoublePoint(3, SimulatorConstants.ROBOT_HEIGHT_METER / 2), 0);
		robot.setFlipY(true);
		
		timerView = new TimerView();
				
		addComponent(robot);
		addComponent(timerView);
	}
	
	public synchronized void setTrajectoryEditor(TrajectoryEditor traj) {
		trajectory = traj;
		restart();
	}
	
	public void setBackgrowndImage(Image backgrownd) {		
		this.backgrownd = backgrownd;
	}
	
	long count = 0;
	
	@Override
	public void paint(Graphics g) {
		if (backgrownd != null) {
			g.drawImage(backgrownd, 0, 0, this);
		} else {
			System.out.println("No Image!");
		}
		
		for (int i = 0; i < drawings.size(); i ++) {
			drawings.get(i).paint(g);
		}
	}
	
	@Override
	public void update(Graphics g) {
		if (i == null) {
			i = createImage(getSize().width, getSize().height);
			doubleG = i.getGraphics();
		}
		
		doubleG.setColor(getBackground());
		doubleG.fillRect(0, 0, getSize().width, getSize().height);
		
		doubleG.setColor(getForeground());
		paint(doubleG);
				
		g.drawImage(i, 0, 0, this);
	}
	
	public void addComponent(Drawable draw) {
		drawings.add(draw);
	}
	
	double startingTime = 0, pauseTime = 0, pathTime;
	
	public synchronized void updateField() {
		if (trajectory == null) {
			return;
		}
		
		double currTime = System.currentTimeMillis();
		if (startingTime == 0) {
			startingTime = currTime;
		}
		
		if (pause) {
			if (pauseTime == 0) {
				pauseTime = currTime;
			}
			
			pathTime = pauseTime - startingTime;
		} else {
			if (pauseTime != 0) {
				startingTime += currTime - pauseTime;
				pauseTime = 0;
			}
			pathTime = currTime - startingTime;
		}
		
		// change to seconds
		pathTime /= 1000;
		
		timerView.setTime(pathTime);
		if (robotMove(pathTime)) {
			pause = true;
			ArrayList<MP_DrivePath> arr = trajectory.getTrajectory().getPathArray();
			double totalTime = 0;
			for (int i = 0; i < arr.size(); i ++) {
				totalTime += arr.get(i).getTotalTime();
			}
			timerView.setTime((double)((int)(totalTime * 1000)) / 1000);
		}
		repaint();
	}
	
	private boolean robotMove(double time){		
		robot.setStartingPosition(trajectory.getStartingPoint(), trajectory.getStartingAngle());
		
		MP_DrivePath tempPath = trajectory.getPathArray().get(0);
		double pathTotalTime =	tempPath.getTotalTime();

		int i = 0;
		while (pathTotalTime <= time) {
			// move robot to end of path
			robot.move(	tempPath.getX(pathTotalTime),
						tempPath.getY(pathTotalTime),
						tempPath.getAngleSimulator(pathTotalTime));
			robot.resetStartingPosition();
		
			// set the time to the time left after this move
			time -= pathTotalTime;
			
			// checks that this is not the last path
			if (i < trajectory.getPathArray().size() - 1) {
				// move to next path
				i ++;
				tempPath = trajectory.getPathArray().get(i);
				pathTotalTime = tempPath.getTotalTime();				
			} else {
				/*
				 * if it got here it means that we got to the end of the trajectory, in
				 * this case we'll put the robot in the ending point of the trajectory.
				 */
				trajectory.setCounter(i);				
				return true;
			}
		}
		
		trajectory.setCounter(i);
		
		robot.move(	tempPath.getX(time),
					tempPath.getY(time),
					tempPath.getAngleSimulator(time));
		return false;
	}
	
	private void restart() {
		pause = true;
		startingTime = System.currentTimeMillis();
		pauseTime = startingTime;
	}
	
	@Override
	public void keyPressed(KeyEvent e) {
		switch (e.getKeyCode()) {
		case KeyEvent.VK_SPACE:
			pause = !pause;
			break;
		case KeyEvent.VK_R:
			restart();
			break;
		case KeyEvent.VK_LEFT:
			pause = true;
			startingTime += 30;
			break;
		case KeyEvent.VK_RIGHT:
			pause = true;
			startingTime -= 30;
			break;
		default:
			break;
		}
	}
	
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub
		
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
		switch (e.getModifiers()) {
		case MouseEvent.BUTTON1_MASK:
			pause = !pause;
			break;
		case MouseEvent.BUTTON3_MASK:
			restart();
			break;
		default:
			break;
		}
		
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseWheelMoved(MouseWheelEvent e) {
		int clicks = e.getWheelRotation();
		
		if (clicks < 0) {
			// up
			pause = true;
			startingTime -= 50;
		} else {
			//down
			pause = true;
			startingTime += 50;
		}
	}
}
