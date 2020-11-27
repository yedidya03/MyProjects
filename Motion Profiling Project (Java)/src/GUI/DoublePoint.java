package GUI;

public class DoublePoint {

	public double x;
	public double y;
		
	public DoublePoint() {
		x = 0;
		y = 0;
	}
	
	public DoublePoint(double x, double y) {
		this.x = x;
		this.y = y;
	}
	
	public void scaleX(double factor) {
		x *= factor;
	}
	
	public void scaleY(double factor) {
		y *= factor;
	}
}
