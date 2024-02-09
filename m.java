import java.io.*;
import java.net.*;
import java.util.Random;

 class irc {


    static String getRD()
    {
   String CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

        int length = 10;
        Random random = new Random();
        StringBuilder sb = new StringBuilder(length);

        for (int i = 0; i < length; i++) {
            int randomIndex = random.nextInt(CHARACTERS.length());
            char randomChar = CHARACTERS.charAt(randomIndex);
            sb.append(randomChar);
        }

        return "nick " + sb.toString();
    }

    public static void s(){
              try {
            // Sleep for 3 seconds
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public static void main(String[] args) {
        try {
            // Create client socket and connect to the server
            Socket clientSocket = new Socket("10.12.6.6", 8080);
            System.out.println("Connected to server.");

            // Create input and output streams for communication
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);

            // Send a message to the server
          //  String message = "\nnick radom\nuser id";
            out.println("pass p");
            s();
            out.println(getRD());
            s();
            out.println("user oth");
            s();
            out.println("join #general");
            s();
            // System.out.println("Sent to server: " + message);

            // Receive the response from the server
            while (true)
            {
            
           
            String response = in.readLine();
            if (response == null)
            break;
            System.out.println("Received from server: " + response);


            }
            // Close the connection
            // clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}