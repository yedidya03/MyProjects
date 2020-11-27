package GUI;

import java.applet.Applet;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Image;

public class AppletMain extends Applet implements Runnable{

	private Image 		i, 
						backgrownd;
	
	private Graphics 	doubleG;
	
	private FieldCanvas field = new FieldCanvas();
	private BlocksPanel blocks = new BlocksPanel();
	private FilesControlPanel filesCpl = new FilesControlPanel();
	
	@Override
	public void init() {
		backgrownd = getImage(getCodeBase(), SimulatorConstants.FIELD_PHOTO_DIRECTORY + SimulatorConstants.FIELD_PHOTO_NAME);
		if (backgrownd != null) {
			field.setBackgrowndImage(backgrownd);
		} else {
			System.out.println("Image didnt upload yet");
		}
		
		setSize(field.getWidth() + blocks.getWidth() + filesCpl.getWidth(), Math.max(field.getHeight(), blocks.getHeight()));
		
		setLayout(new FlowLayout(FlowLayout.LEFT, 0, 0));
		add(blocks);
		add(field);
		add(filesCpl);
		
		Image icon = getImage(getCodeBase(), SimulatorConstants.ICON_PATH);
		
		Frame frame = (Frame)this.getParent().getParent();
		frame.setTitle(SimulatorConstants.TITLE);
		frame.setIconImage(icon);
		frame.setMenuBar(null);

	}
	
	@Override
	public void start() {
		filesCpl.init(field, blocks);
		
		Thread thread = new Thread(this);
		thread.start();
	}
	
	@Override
	public void stop() {
		super.stop();
	}
	
	
	@Override
	public void paint(Graphics g) {
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
	
	@Override
	public void run() {
		while (true) {
			blocks.updatePannel();
			field.updateField();
			sleep(17);
		}
	}
	
	public void sleep(int milis) {
		try {
			Thread.sleep(milis);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
