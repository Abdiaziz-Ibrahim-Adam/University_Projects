import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;


/**
 * A class that represents a maze
 * @author johane
 * @version 2024-03-28
 *
 */
public class Maze {
    private ArrayList<String> theInfo;
    private final Position start;

    private final ArrayList<Position> goals=new ArrayList<>();
    private int numColumns=0;
    private int numRows=0;


    /**
     * Create a maze from a stream
     * @param s scanner to read maze data from
     * @throws StartException if there is no unique start position in the maze
     * @throws NoGoalException if there is no goal position in the maze
     */
    public Maze(Scanner s) throws IOException {
        readMazeData(s);
        start=findStartPosition();
        if(goals.isEmpty()) {
            throw new NoGoalException();
        }
    }

    /**
     * Find the start position of the maze
     * @return the start position of the maze
     * @throws StartException if there is no unique start position in the maze
     */
    private Position findStartPosition() {

        Position start = null;
        for (int y = 0; y < theInfo.size(); y++) {
            for (int x = 0; x < theInfo.get(y).length(); x++) {
                Position position = new Position(x, y);
                if (hasCharacterAtPosition(position, 'S')) {
                    if(start==null)
                        start= position;
                    else {
                        throw new StartException();
                    }
                }
            }
        }
        if(start==null) {
            throw new StartException();
        }
        else {
            return start;
        }
    }



    /**
     * Read the maze data from a stream
     * @param s the scanner to read from
     */
    private void readMazeData(Scanner s)  {

        String str;
        theInfo=new ArrayList<>();
        while(s.hasNext()) {
            str=s.nextLine();
            theInfo.add(str);
            numRows++;
            if(str.length()>numColumns)  {
                numColumns=str.length();
            }
            //Add all goal positions to the goals list
            for(int i=0;i<str.length();i++) {
                if(str.charAt(i)=='G') {
                    goals.add(new Position(i,numRows-1));
                }
            }
        }
    }

    /**
     * Get the number of columns in the maze
     * @return the number of columns in the maze
     */
    public int getNumColumns() {
        return numColumns;
    }

    /**
     * Get the number of rows in the maze
     * @return the number of rows in the maze
     */
    public int getNumRows() {
        return numRows;
    }

    /**
     * Check if a specific position is a goal.
     * @param pos the position that you want to test if it is the goal
     * @return true if pos is one of the goal positions, else false
     */
    public boolean isGoal(Position pos) {
        if(outsideMaze(pos))
            return false;
        return hasCharacterAtPosition(pos, 'G');
    }

    /**
     * Checks if it is possible to move to a specific position
     * @param pos the position in the maze to check
     * @return true if the position pos is movable in the maze, else false
     */
    public boolean isMovable(Position pos) {
        if(outsideMaze(pos))
            return false;

        return hasCharacterAtPosition(pos, ' ') ||
                hasCharacterAtPosition(pos, 'G') ||
                hasCharacterAtPosition(pos, 'S');
    }

    /**
     * Check if a position is outside the maze
     * @param pos the position to check
     * @return true if the position is outside the maze, else false
     */
    private boolean outsideMaze(Position pos) {
        int x=pos.getX();
        int y=pos.getY();
        return x<0||y<0||y>theInfo.size()-1||x>theInfo.get(y).length()-1;
    }

    private boolean hasCharacterAtPosition(Position position, char c) {
        return theInfo.get(position.getY()).charAt(position.getX()) == c;
    }

    /**
     * Get the distance to the closest goal from a specific position.
     * The distance is calculated as the Manhattan distance.
     * @param pos the position to check the distance from
     * @return the manhattan distance to the closest goal
     */
    public int distanceToClosestGoal(Position pos) {
        int minDistance = Integer.MAX_VALUE;
        for (Position goal : goals) {
            int distance = Math.abs(goal.getX() - pos.getX()) + Math.abs(goal.getY() - pos.getY());
            if (distance < minDistance) {
                minDistance = distance;
            }
        }
        return minDistance;
    }


    /**
     * Get the start position of the maze
     * @return position where the start of the maze is located
     */
    public Position getStart() {
        return start;
    }

}
