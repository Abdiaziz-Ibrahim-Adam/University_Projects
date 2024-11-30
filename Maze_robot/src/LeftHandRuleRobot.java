/**
 * This Java class implements a robot that follows the left-hand rule for navigating mazes.
 * According to the left-hand rule, the robot will always attempt to turn left at any junction it encounters.
 * If turning left is not possible, it will move straight if possible; if not, it will turn right.
 * If none of these movements are possible (dead end), it will turn around.
 *
 * @author Abdiaziz Ibrahim Adam
 * @version 25 april 2024
 */
public class LeftHandRuleRobot implements Robot {
    private Position p;
    private Maze m;
    String direction;

    /**
     * Constructor initializes the robot in the maze.
     *
     * @param maze The maze in which the robot will operate, provided during instantiation.
     */
    public LeftHandRuleRobot(Maze maze){
        this.m = maze;
        this.p = maze.getStart();
        direction = "N";
    }

    /**
     * Executes one move of the robot following the left-hand navigation rule.
     * The robot will first attempt to turn left. If not possible, it moves forward.
     * If moving forward is not possible, it turns right.
     * If turning right is also not possible, it turns around.
     */
    public void move() {

        if (m.isMovable(getNextPosition(getLeftDirection()))) {
            turnLeft();
            moveForward();
        }

        else if (m.isMovable(getNextPosition(direction))) {
            moveForward();
        }

        else if (m.isMovable(getNextPosition(getRightDirection()))) {
            turnRight();
            moveForward();
        }

        else {
            turnRight();
            turnRight();
            if (m.isMovable(getNextPosition(direction))) {
                moveForward();
            }
        }
    }

    /**
     * Advances the robot forward in its current direction.
     */
    private void moveForward(){
        p = getNextPosition(direction);
    }

    /**
     * Turns the robot 90 degrees left relative to its current direction.
     */
    private void turnLeft(){
        direction = getLeftDirection();
    }

    /**
     * Turns the robot 90 degrees right relative to its current direction.
     */
    private void turnRight(){
        direction = getRightDirection();
    }

    /**
     * Determines the left direction based on the current direction.
     *
     * @return A string representing the new direction after the turn.
     */
    private String getLeftDirection(){
        switch (direction){
            case "N" : return "W";

            case "W" : return "S";

            case "S" : return "E";

            case "E" : return "N";

            default  : return direction;
        }
    }

    /**
     * Determines the right direction based on the current direction.
     *
     * @return A string representing the new direction after the turn.
     */
    private String getRightDirection(){
        switch (direction){
            case "N" : return "E";

            case "S" : return "W";

            case "E" : return "S";

            case "W" : return "N";

            default  : return direction;
        }
    }

    /**
     * Computes the next position based on a given direction.
     *
     * @param d The direction in which to compute the next position.
     * @return The next position in the maze based on the given direction.
     */
    private Position getNextPosition(String d){
        switch (d){
            case "N" : return p.getPosToNorth();

            case "S" : return p.getPosToSouth();

            case "E" : return p.getPosToEast();

            case "W" : return p.getPosToWest();

            default  : return p;
        }
    }

    /**
     * Retrieves the current position of the robot in the maze.
     *
     * @return The current position of the robot.
     */
    public Position getPosition() {
        return this.p;
    }

    /**
     * Checks if the robot has reached the goal in the maze.
     *
     * @return true if the goal is reached, otherwise false.
     */
    public boolean hasReachedGoal() {
        return m.isGoal(this.p);
    }
}
