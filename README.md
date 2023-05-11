# TdmaMacProtocol-Base Simulation Setup Guide

This guide provides instructions on setting up the OMNeT++ simulation environment and integrating the INET framework to work on simple TDMA Mac protocol. Please follow the steps below:

## Prerequisites
Before proceeding with the setup, make sure you have completed the following installations:

**OMNeT++**: Follow the instructions in the [OMNeT++ Installation Guide](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf)


**INET Framework**: Refer to the [INET Framework Installation Guide](https://inet.omnetpp.org/Installation.html) for installation instructions.


## Opening the IDE
To open the IDE, execute the following command in the terminal:

`$ omnetpp`


In the IDE, select the default workspace, which is typically the "samples" folder that comes with the OMNeT++ installation.

## Importing the Project
1. Clone the project repository to the desired location on your machine.

2. In the IDE, go to **File > Import > General > Existing Projects into Workspace > Next**

3. Select the root directory of the cloned project by clicking on the **Project Explorer**.

4. Click **Finish** to import the project into the workspace.

## Adding the INET Framework
To add the INET framework to your project:

1. In the Project Explorer view, right-click on the project name and select **Properties**.

2. In the Properties window, navigate to **Project References**.

3. If the INET framework is not selected, check the box next to it to include the framework in your project.

Additionally, open the following files in the Project Explorer view and add the following lines at the end:

**inet/src/inet/common/Protocol.cc:**
// INET specific conceptual protocol identifiers
`const Protocol Protocol::hvlsnmac("tdmaMac", "TDMA-Mac");`
**inet/src/inet/common/Protocol.h**
// INET specific conceptual protocol identifiers (in alphanumeric order)
`static const Protocol tdmaMac;`

## Cleaning and Refreshing the Project
In the Project Explorer view, right-click on the project name and select **Clean Project** to clean the project.

After cleaning, right-click on the project name again and select **Refresh** to update the project files.

## Running the Project
To run the project in the IDE:

1. Build the project by selecting **Project > Build all**.

2. Go to **Run > Run** or click on the Run button.

3. Choose **Run As > OMNeT++ Simulation**.

4. Click **OK** and then **OK** again to start the simulation.


Following these steps will set up your environment and allow you to run the project using the INET framework in the OMNeT++ IDE.

Note: Make sure to configure any additional simulation parameters as required by your project.

For further information, refer to the official OMNeT++ and INET documentation.

