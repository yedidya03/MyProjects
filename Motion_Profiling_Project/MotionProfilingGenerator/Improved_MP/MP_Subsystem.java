package Improved_MP;

import MP_ImprovedCommands.MP_Subsystem_PIDOnlyCommand;
import MotionProfiling.MPGains;
import PID_Classes.PID_Gains;
import PID_Classes.Setpoint;
import edu.wpi.first.wpilibj.command.Subsystem;

public abstract class MP_Subsystem extends Subsystem{
	
	protected Setpoint currState;
	protected double defultVmax = 0, defultAcc = 0;

	/*
	 * For a keep in place system : 
	 * 		When created has to be in rest!
	 * 		The first thing that should move it is the commands!
	 */
	protected MP_Subsystem(){
		setCurrState(new Setpoint(getPosition(), 0.0, 0.0));
	}

	/*
	 * Only for a keep in position systems.
	 * 
	 * Put in the constructor to switch to keepInPosition mode.
	 * 
	 * parameters : 
	 * 		pidGains - 	pid gains that are good for keeping the
	 * 					system in a stable position
	 */
	protected void setKeepInPosition(PID_Gains pidGains){
		setDefaultCommand(new MP_Subsystem_PIDOnlyCommand(this, pidGains));
	}
	
	public abstract double getPosition();
	public abstract void setOutput(double output);
	
	/*
	 * Sets the current state userlly in a finish of a privies motion
	 * 
	 * NOTE : 	if the system is a continues system (like moving wheels that
	 *  		need to rotate 1 rotation every time) the pos value should always
	 *  		return to 0 (so in the next motion the movment will be from 
	 *  		this position)
	 */
	public abstract void setCurrState(Setpoint state);
	
	public Setpoint getCurrState(){
		return currState;
	}
	
	public abstract MPGains getDefultGains();
	public abstract double getDefultVmax();
	public abstract double getDefultAcc();
}
