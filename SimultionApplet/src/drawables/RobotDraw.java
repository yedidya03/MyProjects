package drawables;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.awt.geom.Path2D;
import java.awt.geom.RoundRectangle2D;

import GUI.DoublePoint;
import GUI.SimulatorConstants;

public class RobotDraw implements Drawable{
	
	private DoublePoint center = 	new DoublePoint(),		// the center point of the robot
						pivot = 	new DoublePoint(),		// the point the robot pivots around
						startingPivotPoint; 				// the robot pivot starting point
	
	private double 		angle = 	0,
						startingAngle;
	
	private double 		width = 	SimulatorConstants.ROBOT_WIDTH_PIXEL,		
						height = 	SimulatorConstants.ROBOT_HEIGHT_PIXEL;	
	
	private double 		bumpersWidth = 5,
						frontIndicatorStripWidth = width / 2.5;
	
	private boolean		flipY = false;
	
	/*
	 * UNITS :	startingCenterPoint : meters
	 * 			startingAngle		: radians
	 * 
	 * NOTE : startingCenterPoint is the starting point of the CENTER of the robot
	 */
	public RobotDraw(DoublePoint startingCenterPoint, double startingAngle) {
		setStartingPosition(startingCenterPoint, startingAngle);
	}
	
	/*
	 * setStartingPosition(...) sets the position that all the movements will be calculated
	 * according to. After calling this function the function move(...) will place the robot
	 * in the desired place in relation to the starting point seted by this function.
	 * 
	 * UNITS :	startingCenterPoint : meters
	 * 			startingAngle		: radians
	 * 
	 * NOTE  : 	The parameter startingCenterPoint is the starting point of the CENTER of
	 * 			the robot
	 */
	public void setStartingPosition(DoublePoint startingCenterPoint, double startingAngle) {
		this.startingAngle = startingAngle;
		
		DoublePoint pivotFromCenter = coordinatesConvertion(
				SimulatorConstants.ROBOT_PIVOT_POINT_METER.x,
				SimulatorConstants.ROBOT_PIVOT_POINT_METER.y,
				this.startingAngle);
		
		this.startingPivotPoint = new DoublePoint();
		this.startingPivotPoint.x = startingCenterPoint.x + pivotFromCenter.x;
		this.startingPivotPoint.y = startingCenterPoint.y + pivotFromCenter.y;
		
		move(0, 0, 0);
	}
	
	
	/*
	 * resetStartingPosition() sets the starting position to the current position.
	 * 
	 * It is meant for using after every move, because a move can give only the position
	 * from the beginning of the move.
	 */
	public void resetStartingPosition() {
		setStartingPosition(center, this.angle);
	}
	
	
	/*
	 * The function move(...) sets the position of the robot in the orientation given in
	 * relation to the current starting point position.
	 * 
	 * UNITS : 	x, y  : meters
	 * 			angle : radians 
	 */
	public void move(double x, double y, double angle) {
		DoublePoint move = coordinatesConvertion(x, y, startingAngle);
		pivot.x = startingPivotPoint.x + move.x;
		pivot.y = startingPivotPoint.y + move.y;
		
		this.angle = startingAngle + angle;
		
		
		DoublePoint centerFromPivot = coordinatesConvertion(
										-SimulatorConstants.ROBOT_PIVOT_POINT_METER.x,
										-SimulatorConstants.ROBOT_PIVOT_POINT_METER.y,
										this.angle);
		center.x = pivot.x + centerFromPivot.x;
		center.y = pivot.y + centerFromPivot.y;
	}
	
	public void paint(Graphics g) {
		DoublePoint tempCenter = new DoublePoint(center.x, center.y);
		DoublePoint tempPivot = new DoublePoint(pivot.x, pivot.y);
		
		tempCenter.scaleX(SimulatorConstants.FIELD_WIDTH_METER2PIXEL);
		tempCenter.scaleY(SimulatorConstants.FIELD_HEIGHT_METER2PIXEL);
		
		tempPivot.scaleX(SimulatorConstants.FIELD_WIDTH_METER2PIXEL);
		tempPivot.scaleY(SimulatorConstants.FIELD_HEIGHT_METER2PIXEL);
		
		double tempAngle = -angle;
		
		if (flipY) {
			tempCenter.y = (SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL - tempCenter.y);
			tempPivot.y = (SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL - tempPivot.y);
			
			tempAngle *= -1;
		}
		
		// Robot bumpers
		RoundRectangle2D.Double bumpers = new RoundRectangle2D.Double(
				(tempCenter.x - width / 2),
				(tempCenter.y - height / 2), width, height, 7, 7);
		
		// clear robot chasis
		double chasisWidth = width - (bumpersWidth * 2);
		double chasisHeight = height - (bumpersWidth * 2);
		Rectangle r = new Rectangle((int)(tempCenter.x - chasisWidth / 2) + 1,
				(int)(tempCenter.y - chasisHeight / 2) + 1, (int)chasisWidth, (int)chasisHeight);
		
		Path2D.Double robotBumpers = new Path2D.Double();
		robotBumpers.append(bumpers, false);
		robotBumpers.append(r, false);
		
		// Front side
		Rectangle redSideRect = new Rectangle((int)(tempCenter.x - frontIndicatorStripWidth / 2),
				(int)(tempCenter.y - (height / 2)), (int)frontIndicatorStripWidth, (int)bumpersWidth);
		Path2D.Double redSide = new Path2D.Double();
		redSide.append(redSideRect, false);
		
		// Center Dot
		Path2D.Double centerDot = new Path2D.Double();
		int centerPointWidth = 4;
		Rectangle centerDotRect = new Rectangle((int)(tempCenter.x - centerPointWidth / 2),
				(int)(tempCenter.y - (centerPointWidth / 2)),
				(int)centerPointWidth, (int)centerPointWidth);
		centerDot.append(centerDotRect, false);
		
		// Pivot Dot
		Path2D.Double pivotDot = new Path2D.Double();
		double pivotDotWidth = 6;
		Rectangle pivotDotRect = new Rectangle((int)(tempPivot.x - pivotDotWidth / 2),
				(int)(tempPivot.y - (pivotDotWidth / 2)),
				(int)pivotDotWidth, (int)pivotDotWidth);
		pivotDot.append(pivotDotRect, false);
		
		// Rotate
		AffineTransform t = new AffineTransform();
		t.rotate(tempAngle, tempCenter.x, tempCenter.y);
		robotBumpers.transform(t);
		redSide.transform(t);
		centerDot.transform(t);
		
		// Draw
		Graphics2D g2 = (Graphics2D)g.create();
		g2.setColor(new Color(60, 189, 66));
		g2.fill(robotBumpers);
		
		g2.setColor(Color.RED);
		g2.fill(redSide);
		
		g2.setColor(Color.YELLOW);
		g2.fill(centerDot);
		
		g2.setColor(Color.blue);
		g2.fill(pivotDot);
		
		
		g2.dispose();
	}
	
	public void setFlipY(boolean flip) {
		flipY = flip;
	}
	
	public static DoublePoint coordinatesConvertion(double x, double y, double angle) {
		DoublePoint ret = new DoublePoint();
		ret.x = x * Math.cos(angle) + y * Math.sin(angle);
		ret.y = y * Math.cos(angle) - x * Math.sin(angle);
		
		return ret;
	}
}
