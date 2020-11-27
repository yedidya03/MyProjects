package GUI;

import java.awt.Color;
import java.net.URL;

public class SimulatorConstants {

	
	// The path of the icon.
	public static final String ICON_PATH = 
			".\\..\\static\\robot_icon.png";
	
	// The title of the window
	public static final String TITLE = "The Y Team - Trajectory Builder 2019";
	
	/*
	 * FILES_DIRECTORY is the folder where the path files are stored.
	 */
	
	// The directory in witch the trajectory files are stored in. Relative to the base directory.
	public static final String FILES_DIRECTORY = 
			".\\..\\res\\Trajectories";
	
	/*
	 * Field Constants
	 */

	// The directory in witch the field photo is stored in. Relative to the base directory.
	public static final String FIELD_PHOTO_DIRECTORY =
			".\\..\\res\\Field_Picture\\";
	// the name of the picture of the field
	public static final String FIELD_PHOTO_NAME = "HalfField2019.png";
	
	/*
	 * FIELD_WIDTH and FIELD_LENGTH should be the width and length of 
	 * the part of the field that is showed in the picture (in meters).
	 */
	public static final double FIELD_WIDTH = 8.23;
	public static final double FIELD_HEIGHT = 8.26;
	
	public static final int FIELD_PICTIRUE_WIDTH_PIXEL = 656;
	public static final int FIELD_PICTIRUE_HEIGHT_PIXEL = 662;
	
	
	// DON'T NEED TO FILL
	public static final double FIELD_WIDTH_METER2PIXEL = FIELD_PICTIRUE_WIDTH_PIXEL / FIELD_WIDTH;
	public static final double FIELD_HEIGHT_METER2PIXEL = FIELD_PICTIRUE_HEIGHT_PIXEL / FIELD_HEIGHT;
	
	
	/*
	 * ROBOT Constants
	 */
	
	/*
	 * ROBOT_WIDTH and ROBOT_LENGTH should be the width and length of the
	 * robot WITH bumpers in meters.
	 */
	public static final double ROBOT_WIDTH_METER = 0.86;
	public static final double ROBOT_HEIGHT_METER = 1;
	
	
	// DON'T NEED TO FILL
	public static final double ROBOT_WIDTH_PIXEL = 	SimulatorConstants.ROBOT_WIDTH_METER
													* SimulatorConstants.FIELD_WIDTH_METER2PIXEL;
	
	public static final double ROBOT_HEIGHT_PIXEL = SimulatorConstants.ROBOT_HEIGHT_METER
													* SimulatorConstants.FIELD_HEIGHT_METER2PIXEL;

	/*
	 * ROBOT_PIVIT_POINT is the pivot point of the robot in relation to the center of it.
	 * NOTE : 	In the x aces + is right and - is left.
	 * 			In the y aces + is up (front) and - is down (rear).
	 */
	public static final DoublePoint ROBOT_PIVOT_POINT_METER = new DoublePoint(0, -0.0);
	
	// DON'T NEED TO FILL
	public static final DoublePoint ROBOT_PIVOT_POINT_PIXEL = 
			new DoublePoint(ROBOT_PIVOT_POINT_METER.x * FIELD_WIDTH_METER2PIXEL,
					ROBOT_PIVOT_POINT_METER.y * FIELD_HEIGHT_METER2PIXEL);
	
	
	public static final Color REGULAR_COLOR = new Color(157, 164, 172);
	public static final Color CHOSEN_COLOR = new Color(120, 204, 96);
	public static final Color CONTROL_PANEL_BACKGROWND_COLOR = new Color(100, 172, 200);
	public static final Color TIMER_COLOR = Color.GREEN;

	
}
