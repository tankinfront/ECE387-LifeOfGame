% Matalab code for processing pictures from a folder on the computer hold
% in "MyFolder" variable. The rpogram will load all th epictures and will
% search for a circular object on them with a user specified in the "Sizes"
%variable. After the centers are collected they are send to the arduino Uno
%with an Xbee wireles module to be send to the robot car.
%Auther: Stanislav Stanchev
%References:....
clc; clear;
delete(instrfindall); %Deletes any open serial communication before establishing one
%Arduino objects

arduino = serial('COM9','BaudRate',9600); % create serial communication object
arduinoMega = serial('COM10','BaudRate',9600); %Mega board connected to transfer the
goToPositionX = 0;
goToPositionY = 0;
counter = 1;
%Variables that help with indexing and last values.
Xindex = 1; %indexing for the above matrix
currentRow = 0; %Keep track of the last row that was saved
currentColumn = 0; %Keep track of the last column that was saved
Xint = 0;
Yint = 0;
xyPosition = "";
goToPosition = "";
%Variable to switch between scanning pictures and sending trought the
%serial port
receivedData = false;
sizeOfData = 100 ;
%Data recieved from Arduino
recievedData = zeros(1,sizeOfData);



for majorLoop = 1:500000000
    if (receivedData)
        
        % Read 1.jpg through m.jpg. Files are in the the following directory.
        MyFolder = 'C:\Users\Stanc\Downloads\MathWorks\SupportPackages\R2017b\Pictures';
        
        
        filePattern = fullfile(MyFolder, '*.jpg'); % Get all the files that have .jpg.
        %Collect all the pictures in a struct
        theFiles = dir(filePattern);
        out = size(theFiles,1);
        
        %Container for all coordinates
        xyMatrix = zeros(length(theFiles),2) ;
        
        %Estimation of the radius of the object to be searched for.
        Sizes = [16 21];
        
        %Counter used to keep the order of the collected center coordinates
        count_saved_variables = 0;
        for k = 1 : length(theFiles)
            
            %Next files name is collected.
            baseFileName = theFiles(k).name;
            %image object made from the name and folder that contains it.
            fullFileName = fullfile(MyFolder, baseFileName);
            
            % Process the image
            imageArray = imread(fullFileName);
            
            %Function detects a circle and returns the center and the radius of the
            %circle.
            [centers, radii] = imfindcircles(imageArray,Sizes,'ObjectPolarity','dark','Sensitivity',0.95);
            [rows, columns] = size(centers); %turns the centers saved in the vector to x and y coordinates
            
            %If no coordinates are found jumps over the if statement.
            %No duplicates are recorded in the container.
            if(  size(centers) > 0 )
                %Check if the image array is almost process
                
                %Collect the the coordinates.
                xyMatrix (Xindex, 1)  = centers(1) ;
                xyMatrix (Xindex, 2)  = centers(2);
                Xindex = Xindex+1;
                counter = counter +1;%will help get rid of the empty slots to save some space
                
                currentRow = centers(1);
                currentColumn = centers(2);
                
            end %End if statement
            
            
        end
        Xint = round(currentRow);
        Yint = round(currentColumn);
        
        
        %String of the position coordinates
        xyPosition =  num2str(Xint) + num2str(Yint);
        
        %Message to be send with both goToPosition and current position
        payload = goToPosition + xyPosition;
        
        %Send the coordinates to the arduino with Xbee.
        fopen(arduino);
        fprintf(arduino, '%s\n', payload);
        
        fclose(arduino);
        
        %Display is used for testing purposes
        %disp(xyPosition);
        %disp(Xint);
        %disp(currentColumn);
        %disp(Yint);
        %delete imageArray;
        receivedData = false;
        break;
        
    else
        
        %Serial Cumunication Receive the coordinates from the screen.
        value = zeros(1,2);
        
        fopen(arduinoMega);
        for i=1:2
            value(i) = fscanf(arduinoMega,'%f\n');
            
        end
        
        %Update the coordinates that need to be send.
        goToPositionX = value(1);
        goToPositionY = value(2);
        goToPosition  = num2str(goToPositionX) + num2str(goToPositionY);
        %This is used to switch between sending and receiving of serial
        %data
        
        receivedData = true;
        fclose(arduinoMega);
        
        disp(goToPositionX);
        disp(goToPositionY);
        %disp(value);
        
       
        
    end     %End the switching if statement
    
end     %End majoLoop
