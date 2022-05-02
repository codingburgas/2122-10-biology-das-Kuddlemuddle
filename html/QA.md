# QA Documentation
# Introduction
The job of the QA engineer is very important, as he has to check if the code is correct. It’s not an easy job, as you always have to think of new ways to “break” the program so most of the problems in the code are corrected.

# Entry and exit criteria
## Entry Criteria
When the QA engineer starts his work, the program isn’t optimized and “cleared” from any problems it may have.

## Exit Criteria
When the QA engineer finished his work, the program is more optimized and up-to-date with less problems.

# Objectives and tasks
## Objective
The goal is that we end up with a complete and working product.

## Tasks
The tasks a QA engineer has to face are:
1.	Preparation of a QA documentation
2.	Testing the app
3.	Reports any problems
4.	Making a documentation about the corresponding problem

# Scope
The program has to be fully tested so any problems can be reported and solved.

# Test deliverables
The result of a tested program are that there will be fewer malfunctions and it will be more accessible for use.

# Test Cases
## Validation of the data
### Test Case 1
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-1|Created by|Mariya Ilcheva| | |
| | |Name|Is email valid| | | | |
| | |Description|Checks whether the email entered by the user is valid|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|Enter invalid email.|mzilcheva|The program does not accept the invalid email and outputs the following message "Please, enter a valid email!" till the user enters a valid one|The program does not accept the invalid email and outputs the following message "Please, enter a valid email!" till the user enters a valid one|PASSED|
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |2|Enter valid email.|MZIlcheva19@cb.bg|The program accepts the email|The program accepts the email|PASSED|

### Test Case 2
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-2|Created by|Mariya Ilcheva| | |
| | |Name|Is username valid| | | | |
| | |Description|Checks whether the username entered by the user is valid|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|Enter invalid username|232323|The program does not accept the invalid username and outputs the following message "Please, enter a valid username" till the user enters a valid one|The program does not accept the invalid username and outputs the following message "Please, enter a valid username" till the user enters a valid one|PASSED|
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |2|Enter valid username|Mariya143|The program accepts the username|The program accepts the username|PASSED|
### Test Case 3
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-3|Created by|Mariya Ilcheva| | |
| | |Name|Is password valid| | | | |
| | |Description|Checks whether the password entered by the user is valid|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|Enter invalid password|sdkhbuw2|The program does not accept the invalid password and outputs the following message "Please, enter a valid password!" till the user enters a valid one|The program does not accept the invalid password and outputs the following message "Please, enter a valid password!" till the user enters a valid one|PASSED|

## Visualization of the data
### Test Case 1
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-4|Created by|Mariya Ilcheva| | |
| | |Name|Team logo| | | | |
| | |Description|Check if the team logo is printed when the program is started|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user runs the program|#|The team logo is printed on the console|The team logo is printed on the console|PASSED|

### Test Case 2
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-5|Created by|Mariya Ilcheva| | |
| | |Name|Loading page| | | | |
| | |Description|Check if the loading page is printed when the program is started|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user runs the program|#|The team loading page is printed on the console|The team loading page is printed on the console|PASSED|

### Test Case 3
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-6|Created by|Mariya Ilcheva| | |
| | |Name|Closed book| | | | |
| | |Description|Check if the closed book is printed when the program is started|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user runs the program|#|The team closed book is printed on the console|The team closed book is printed on the console|PASSED|

### Test Case 4
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-7|Created by|Mariya Ilcheva| | |
| | |Name|Snake sword| | | | |
| | |Description|Check if the snake sword is printed on the closed book when the program is started|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user runs the program|#|The snake sword is printed on the closed book|The snake sword is printed on the closed book|PASSED|

### Test Case 5
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-8|Created by|Mariya Ilcheva| | |
| | |Name|Book decorations| | | | |
| | |Description|Check if the book decorations are printed on the closed book when the program is started|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user runs the program|#|The book decorations are printed on the closed book|The book decorations are printed on the closed book|PASSED|

### Test Case 6
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-9|Created by|Mariya Ilcheva| | |
| | |Name|Print event as encyclopedia| | | | |
| | |Description|Check if the event data is printed as an encyclopedia|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user chooses the option "Print as encyclopedia"|#|The encyclopedia with the event data is printed on the console|The encyclopedia with the event data is printed on the console|PASSED|

### Test Case 7
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-10|Created by|Mariya Ilcheva| | |
| | |Name|Print event as map| | | | |
| | |Description|Check if the event data is printed as a map|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user chooses the option "Print as map"|#|The map with the event data is printed on the console|The map with the event data is printed on the console|PASSED|

### Test Case 8
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-11|Created by|Mariya Ilcheva| | |
| | |Name|Print event as timeline| | | | |
| | |Description|Check if the storyline is correctly printed on the timeline with year and title|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user chooses to display a storyline as timeline|#|The timeline is printed with the events' years and titles|The timeline is printed with the events' years and titles|PASSED|

### Test Case 9
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-12|Created by|Mariya Ilcheva| | |
| | |Name|Users visualization| | | | |
| | |Description|Check if all the users are printed on the console|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user chooses to display all the users|#|All the users are printed on the console|All the users are printed on the console|PASSED|

### Test Case 10
| | | | | | | | |
|-|-|-|-|-|-|-|-|
| | |ID|LTH-13|Created by|Mariya Ilcheva| | |
| | |Name|Book opening animation| | | | |
| | |Description|Check if when a menu option is choosed the book on the console is opening|Created on|15/02/2022| | |
| | | | | | | | |
| | |#|Description|Test Data|Expectations|Actual Result|Status|
| | |1|The user chooses an option from the menu|#|The book animation is played on the console|The book animation is played on the console|PASSED|