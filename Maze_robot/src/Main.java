import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    /**
     * Old testprogram
     * @param args command line arguments. args[0] should be the path to the maze file
     */
    public static void main(String[] args) {
        try {

            // Maze setup
            Maze maze = new Maze(new Scanner(new File(args[0])));

            // Robot setup
            int stepCount = 0;
            RandomRobot randomRobot = new RandomRobot(maze);

            //Loop while robot has not found the goal symbol
            while (!randomRobot.hasReachedGoal()) {
                randomRobot.move();
                System.out.println(randomRobot.getPosition());
                stepCount++;
            }

            System.out.println("Robot moved " + stepCount + " steps to reach Goal.");

        } catch (IOException e) {
            System.out.println("File could not be found/read");
        } catch (NoGoalException e) {
            System.out.println("Maze contained no goal");
        } catch (StartException e) {
            System.out.println("Maze contained no start");
        }
    }

}
