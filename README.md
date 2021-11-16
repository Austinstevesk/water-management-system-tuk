# water-management-system-tuk
Water management system Tuk_IOT  

# Setup Instructions  
- Clone the repository  
- Open the completewaterflowsensor file on arduino  
- You can edit the last function to match the number of econds you want the pump to run;
  By default, its in milliseconds so if you want it to run for 10 secs, edit the line ```desired_stop_millis = millis() + 3000```
  to have ```desired_stop_millis = millis() + 10000```.  
- ```led variable``` should be changed to ```relay```.  
- Load the code to arduino and verify  
- Upload the code to the board with the components already set up  
- Simulate people getting in and out using the two ultrasonics  
