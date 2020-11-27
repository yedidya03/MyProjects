package GUI;

import java.util.ArrayList;

import Improved_MP.MP_DrivePath;
import MP_ImprovedCommands.MP_AutoGenerator;

public class TrajectoryEditor {

	private DoublePoint 				startingPosition;
	private double 						startingAngle;
	
	private MP_AutoGenerator 			trajectory;
	
	private ArrayList<MP_DrivePath> 	pathArray;
	
	private int 						counter = 0;
	
	public TrajectoryEditor() {
		trajectory = new MP_AutoGenerator();
	}
	
	public void setStartingPoint(DoublePoint position, double angle) {
		startingPosition = position;
		startingAngle = Math.toRadians(angle);
	}
	
	public void setTrajectory(MP_AutoGenerator traj) {
		trajectory = traj;
	}
	
	public MP_AutoGenerator getTrajectory() {
		return trajectory;
	}
	
	public ArrayList<MP_DrivePath> getPathArray(){
		if (pathArray == null) {
			pathArray = trajectory.getPathArray();
		}
		
		return pathArray;
	}
	
	public void setCounter(int i) {
		counter = i;
	}
	
	public int getCounter() {
		return counter;
	}
	
	public void setStartingPoint(DoublePoint start) {
		startingPosition = start;
	}
	
	public DoublePoint getStartingPoint() {
		return startingPosition;
	}

	public double getStartingAngle() {
		return startingAngle;
	}
}
