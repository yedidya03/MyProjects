package drawables;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Component;
import java.awt.Event;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import GUI.SimulatorConstants;

public class TextCanvas extends Canvas {
	
	private static final int BACKGROWND_RADIUS = 3;
	
	private Image 		i;
	private Graphics 	doubleG;
	
	private int width, height = 20, fontSize = 12;
	private String text;
	
	private Color backgrownd, frame, textColor, perm;
	
	public TextCanvas(String text, int width) {
		this.text = text;
		this.width = width;
		
		backgrownd = SimulatorConstants.REGULAR_COLOR;
		perm = backgrownd;
		frame = Color.BLACK;
		textColor = Color.BLACK;
				
		setSize(width + 1, height + 1);
	}
	
	public void setBackgrowndColor(Color backgrownd) {
		this.backgrownd = backgrownd;
		repaint();
	}
	
	@Override
	public void paint(Graphics g) {
		g.setColor(backgrownd);
		g.fillRoundRect(0, 0, width, height, BACKGROWND_RADIUS, BACKGROWND_RADIUS);
		
		g.setColor(frame);
		g.drawRoundRect(0, 0, width, height, BACKGROWND_RADIUS, BACKGROWND_RADIUS);
		
		g.setFont(new Font("calibri", Font.BOLD, fontSize));
		g.setColor(textColor);
		g.drawString(text, 10, (height / 2) + (fontSize / 3));
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
