import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.Scanner;
import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * Test class for {@link LeftHandRuleRobot}.
 */
public class LeftHandRuleRobotTest {
    private Maze maze;
    private LeftHandRuleRobot robot;

    /**
     * Tests the move method of the LeftHandRuleRobot class.
     * Verifies that the robot moves according to the left-hand rule.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void MoveTest() throws IOException {
        String mazeLayout =
                """
                        ************
                        *          *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        *S     *G  *
                        ************
                        """;

        maze = new Maze(new Scanner(mazeLayout));
        robot = new LeftHandRuleRobot(maze);

        robot.move();
        Position expectedPos1 = new Position(2,5);

        assertEquals(expectedPos1, robot.getPosition());

        robot.move();
        Position expectedPos2 = new Position(3,5);

        assertEquals(expectedPos2, robot.getPosition());
    }

    /**
     * Tests the getPosition method of the LeftHandRuleRobot class.
     * Verifies that the getPosition method returns the correct position of the robot.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void getPositionTest() throws IOException {
        String mazeLayout =
                """
                        ************
                        *          *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        *S     *G  *
                        ************
                        """;

        maze = new Maze(new Scanner(mazeLayout));
        robot = new LeftHandRuleRobot(maze);

        assertEquals (new Position(1,5),robot.getPosition(),"The Start coordinates is (1,5) So I should get those coordinates!");
    }

    /**
     * Tests the hasReachedGoal method of the LeftHandRuleRobot class.
     * Verifies that the robot correctly detects reaching the goal.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void HasReachedGoal() throws IOException {
        String mazeLayout =
                """
                        ************
                        *          *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        *S     *G  *
                        ************
                        """;

        maze = new Maze(new Scanner(mazeLayout));
        robot = new LeftHandRuleRobot(maze);

        while(!robot.hasReachedGoal()){
            robot.move();
        }
        assert (robot.hasReachedGoal());
    }
}
