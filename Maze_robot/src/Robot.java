/**
 * Defines the basic functionalities for a robot navigating in a maze.
 * This interface specifies methods for moving the robot, retrieving its position,
 * and checking if it has reached its goal.
 *
 * @author Abdiaziz Ibrahim Adam
 * @version 25 april 2024
 */
public interface Robot{

    /**
     * This instructs the robot to perform a move operation. The specific movement behavior
     * (like how and where it moves) is determined by the implementing class.
     */
    void move();

    /**
     * Retrieves the current position of the robot within the maze.
     *
     * @return The current position of the robot as a {@link Position} object.
     */
    Position getPosition();

    /**
     * Determines whether the robot has successfully reached its goal in the maze.
     *
     * @return true if the robot has reached the goal, false otherwise.
     */
    boolean hasReachedGoal();
}