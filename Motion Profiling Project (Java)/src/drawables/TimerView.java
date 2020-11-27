package drawables;

import java.awt.Font;
import java.awt.Graphics;

import GUI.SimulatorConstants;

public class TimerView implements Drawable{

	private double time = 0;
	
	public void setTime(double time) {
		this.time = time;
	}
	
	@Override
	public void paint(Graphics g) {
		g.setFont(new Font("Courier New", 1, 17));
		g.setColor(SimulatorConstants.TIMER_COLOR);
		g.drawString("" + time, SimulatorConstants.FIELD_PICTIRUE_WIDTH_PIXEL - 65,
				SimulatorConstants.FIELD_PICTIRUE_HEIGHT_PIXEL - 40);
	}

}
