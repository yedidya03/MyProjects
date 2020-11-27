package filesHandling;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import GUI.DoublePoint;
import GUI.TrajectoryEditor;

public class TrajectoryFile {

	private static final String START_SING = "start:";
	private static final String STRAIGHT_SING = "addStraight(";
	private static final String RADIUS_SING = "addRadius(";
	
	FileInputStream in;
	FileOutputStream out;
	
	File file;
	
	public TrajectoryFile(File file) {
		this.file = file;
	}
	
	public TrajectoryEditor readFile() throws Exception {
		TrajectoryEditor trajectoryEditor = new TrajectoryEditor();
		String line;
			
		try {
			in = new FileInputStream(file);
			BufferedReader buff = new BufferedReader(new InputStreamReader(in));

			int i = 1;
			while ((line = buff.readLine()) != null) {
				//System.out.println(line);
				pharsLine(trajectoryEditor, line, i);
				i ++;
			}
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return trajectoryEditor;
	}
	
	private boolean pharsLine(TrajectoryEditor trajectoryEditor, String line, int lineNumber) throws Exception {
		line = line.replaceAll("\\s+", "");
		
		double[] params;
		
		if (line.equals("")) {
			return true;
		}
		
		if (line.charAt(0) == '#') {
			return true;
		}	
		
		
		if (line.substring(0, START_SING.length()).equals(START_SING)) {
			params = getParameters(line.substring(START_SING.length(), line.length()), lineNumber);
			
			if (params.length != 3) {
				throw new Exception(
						"ERROR : in file line " + lineNumber + " : wrong number of parameters.");
			}
			
			trajectoryEditor.setStartingPoint(new DoublePoint(params[0], params[1]), params[2]);
			
			return true;
		}
		
		if ((params = handleGeneratorLine(line, STRAIGHT_SING, lineNumber)) != null) {
			switch (params.length) {
			case 2:
				trajectoryEditor.getTrajectory().addStraight(params[0], params[1]);
				break;
				
			case 6:
				trajectoryEditor.getTrajectory().addStraight(	params[0],
																params[1],
																params[2],
																params[3],
																params[4],
																params[5]);
				break;
			default:
				throw new Exception(
						"ERROR : in file line " + lineNumber + " : wrong number of parameters.");
			}
			return true;
		}
		
		if ((params = handleGeneratorLine(line, RADIUS_SING, lineNumber)) != null) {
			switch (params.length) {
			case 3:
				trajectoryEditor.getTrajectory().addRadius(		params[0],
																params[1],
																params[2]);
				break;
				
			case 7:
				trajectoryEditor.getTrajectory().addRadius(	params[0],
															params[1],
															params[2],
															params[3],
															params[4],
															params[5],
															params[6]);
				break;
			default:
				throw new Exception("ERROR : in file line " + lineNumber + " : wrong number of parameters.");
			}
			return true;
		}
		
		throw new Exception("ERROR : in file line " + lineNumber + " : line is not legal");
	}
	
	private double[] handleGeneratorLine(String line, String sign, int lineNumber) throws Exception {
		if (line.substring(0, sign.length()).equals(sign)) {
			if (line.length() < sign.length() + 2) {
				throw new Exception("ERROR : in file line " + lineNumber + " : line is to short");
			}
			
			if (!line.substring(line.length() - 2, line.length()).equals(");")){
				throw new Exception("ERROR : in file line " + lineNumber + " : there is no \');\' at the end of line");
			}
			
			return getParameters(line.substring(sign.length(), line.length() - 2), lineNumber);
		}
		
		return null;
	}
	
	private double[] getParameters(String parametersPlace, int lineNumber) throws Exception {
		String[] params = parametersPlace.split(",");
		double[] ret = new double[params.length];
		
		for (int i = 0; i < params.length; i ++) {
			double temp = 0;
			try {
				temp = Double.parseDouble(params[i]);
			} catch (Exception e) {
				throw new Exception("ERROR : in file line " + lineNumber + " : wrong parameter : " + params[i]);
			}
			ret[i] = temp;
		}
		
		return ret;
	}
	
}
