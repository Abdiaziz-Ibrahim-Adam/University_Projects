//Class providing  a main method that presents a gui
//that simulates robots moving in a maze

//package gui;


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

/*
import model.Maze;
import model.HomingRobot;
import model.Position;
import model.RandomRobot;
import model.LeftHandRuleRobot;
import model.Robot;
*/


/**
 * Graphical robot simulator
 * @author johane
 *
 */
public class MazeSimulation {

    public enum RobotType {Random,Homing,LeftHand}

    /**
     * Maximal number of steps that the simulation will run
     */
    public static final int MAX_STEPS=300;
    private DisplayPanel[][] mazeDisplay;
    private Maze maze;
    private JTextField messageField;
    private JPanel mazePanel;

    /**
     * 
     */
    public MazeSimulation() {

        JFrame theWindow;
        theWindow = setupMainWindow();
        JMenuBar menuBar = createMenuBar();
        theWindow.setJMenuBar(menuBar);
        
        mazePanel = new JPanel();
        theWindow.add(mazePanel,BorderLayout.CENTER);
        messageField = new JTextField();
        theWindow.add(messageField,BorderLayout.SOUTH);
        
        JPanel topPanel = createTopPanel();
        theWindow.add(topPanel,BorderLayout.NORTH);
        theWindow.setVisible(true);
        loadMaze();
       

    }

    
    /**
     * Create and configure the menubar for the application
     * @return the configured menu bar redy to be added to the userinterface.
     */
    protected JMenuBar createMenuBar() {
        JMenuBar menuBar=new JMenuBar();
        JMenu file=new JMenu("File");
        JMenuItem open=new JMenuItem("Open");
        open.addActionListener(e->this.loadMaze());
        JMenuItem quit=new JMenuItem("Quit");
        quit.addActionListener(e->System.exit(0));
        file.add(open);
        file.add(quit);
        JMenu robot=new JMenu("Robot");
        menuBar.add(file);
        menuBar.add(robot);
        return menuBar;
    }

    /**
     * Create the top part of the user interface with options to select which
     * type of robot to simulate
     * @return panel containing this part of the userinterface
     */
    protected JPanel createTopPanel() {
        JPanel topPanel=new JPanel();
        topPanel.setLayout(new GridLayout(1,0));
        JButton randomButton = new JButton("RandomRobot");
        topPanel.add(randomButton);
        JButton homingButton = new JButton("HomingRobot");
        topPanel.add(homingButton);
        JButton leftButton = new JButton("LeftHandRuleRobot");
        topPanel.add(leftButton);
        JButton clearButton = new JButton("Clear");
        clearButton.addActionListener(e->setupMazePanel());
        topPanel.add(clearButton);
        
        //Configure button actions
        ActionListener randomRobot=new RobotButtonListener(
                RobotType.Random);
        randomButton.addActionListener(randomRobot);
        ActionListener homingRobot=new RobotButtonListener(
                RobotType.Homing);
        homingButton.addActionListener(homingRobot);
        ActionListener leftRobot=new RobotButtonListener(
                RobotType.LeftHand);
        leftButton.addActionListener(leftRobot);
               
        return topPanel;
    }

    /**
     * Prompts the user for a file to open and displays that maze file in the
     * gui
     */
    protected void loadMaze() {
        JFileChooser fileChooser;
        File file=null;
        boolean done=false;
        //Load maze
        fileChooser=new JFileChooser();
        FileFilter filterMaze = new FileNameExtensionFilter("Maze file", "maze");
        FileFilter filterText = new FileNameExtensionFilter("Text file", "txt");
        fileChooser.addChoosableFileFilter(filterMaze);
        fileChooser.addChoosableFileFilter(filterText);
        do {
            int returnVal=fileChooser.showOpenDialog(null);
            if (returnVal == JFileChooser.APPROVE_OPTION) {
                file = fileChooser.getSelectedFile();
            } else {
                System.exit(0); //Canceled by user
            }
            try {
                //Create a maze from a text file
                maze=(new Maze(new Scanner(file)));
                mazeDisplay=new DisplayPanel[maze.getNumRows()][maze.getNumColumns()];
                mazePanel.setLayout(new GridLayout(maze.getNumRows(),maze.getNumColumns()));
                done=true;
            } catch (FileNotFoundException e) {
                updateMessage("Unable to open maze file");
            }
            catch (Exception e) {
                updateMessage("Something wrong with map");
            }
        } while(!done);
        setupMazePanel();
    }

    /**
     * Create and configure the main window
     * @return the window
     */
    protected JFrame setupMainWindow() {
        JFrame theWindow;
        //Setup main window
        theWindow = new JFrame("RoboSim");
        theWindow.setSize(500, 500);
        theWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        theWindow.setBackground(Color.WHITE);
        return theWindow;
    }

    /**
     * Display maze in window
     */
    private void setupMazePanel() {
        mazePanel.removeAll();
        for(int i=0;i<maze.getNumRows();i++) {
            for(int j=0;j<maze.getNumColumns();j++) {
                DisplayPanel dp=new DisplayPanel();
                if(!maze.isMovable(new Position(j,i)))
                    dp.setWall();
                else if(maze.isGoal(new Position(j,i)))
                    dp.setGoal();
                else
                    dp.setEmpty();
                mazePanel.add(dp);
                mazeDisplay[i][j]=dp;
            }
        }
        mazePanel.doLayout();
    }

    /**
     * Simulate a robot moving in the maze
     * @param r the robot to simulate
     */
    public void runSimulation(Robot r) {

        Position robotPos=r.getPosition();
        displayRobot(robotPos);
        int i=0;
        do { //Simulate the robot moving
            removeRobot(r.getPosition());
            r.move();
            displayRobot(r.getPosition());
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                return;
            }
            i++;
        } while(!r.hasReachedGoal() && i<MAX_STEPS);
        if(r.hasReachedGoal())
            updateMessage("Robot reached goal after "+i+" steps");
        else
            updateMessage("Robot did not reach goal before reaching maximum number of steps");
    }

    /**
     * Notify the user about something. May be called on any thread
     * @param string the message to the user
     */
    private void updateMessage(final String string) {
        SwingUtilities.invokeLater(()->messageField.setText(string));
    }

    /**
     * Remove the robot from the specified position in the gui
     * @param pos the position were a robot should be removed
     */
    private void removeRobot(final Position pos) {
        SwingUtilities.invokeLater( 
                ()->mazeDisplay[pos.getY()][pos.getX()].setEmpty());
    }

    /**
     * Display a robot in the given position
     * @param robotPos
     */
    private void displayRobot(final Position robotPos) {
        SwingUtilities.invokeLater(
                ()-> mazeDisplay[robotPos.getY()][robotPos.getX()].setRobot());
    }

    class RobotButtonListener implements ActionListener {
    
        private RobotType type;
    
        /**
         * @param r
         */
        public RobotButtonListener(RobotType r) {
            type=r;
        }
    
        /* (non-Javadoc)
         * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
         */
        @Override
        public void actionPerformed(ActionEvent event) {
            new Thread(new Runnable(){
    
                @Override
                public void run() {
                    try {
                        switch(type) {
                            case Homing:
                                runSimulation(new HomingRobot(maze));
                            break;
                            case LeftHand:
                                runSimulation( new LeftHandRuleRobot(maze));
                            break;
                            case Random:
                                runSimulation(new RandomRobot(maze));
                            break;
                        }
                    } catch(Exception e) {
                        updateMessage("Simulation failed because of "+
                            e.getClass().getName());
                    }
    
                }}).start();
        }
    }
    
    /**
     * A simple panel able to display one of the things available in a simulation
     * of a robot walking in a maze. In this simple version the different objects
     * are only displayed as different colored tiles
     * @author johane
     *
     */
    public static class DisplayPanel extends JPanel {

        /**
         * ID for serialization
         */
        private static final long serialVersionUID = 1L;

        public void setWall() {
            this.setBackground(Color.BLACK);
        }

        public void setEmpty() {
            this.setBackground(Color.WHITE);
        }

        public void setRobot() {
            this.setBackground(Color.RED);
        }

        public void setGoal() {
            this.setBackground(Color.GREEN);
        }

    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(()->new MazeSimulation());
    }
}
