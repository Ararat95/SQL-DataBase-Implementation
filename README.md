# This is MY SQL DataBase system own implementation 


**THIS FILE CONTAINS OF DB FILES IN DB FOLDER AND SOURCE FILES IN SRC FOLDER**

***To use all this it's important to follow syntax to get real and neccessary result***

**Instructions are insert, delete, update, select**
**For all DBs you need to know main instruction calls, see below**
## Insert function
**insert into /DB file name/ data input** 
*for example*
### In User DB 
 *insert into user.data name = name(neccessary), surname = surname(neccessary), age = 20(optional), email = user@gmail.com(neccessary), password = somepass(optional), position = new position(optional), mobile = 091101202(optional)
 put '=' always between data's name and data's part, and put ',' always between each kind of data, some changes in task and project i'll let you know later.*
 
### In Task DB 

  *insert into task.data project = project(neccessary), title = title(neccessary), description = <some description>(optional), users = <some user email>
  (optional), deadline = 25.12.2022(format is this for all deadlines)(optional).
  Some changes for users, and description, use <> scopes to encapsulate all data and not to have any other problems later.*
  
### In Project DB
  **Here are some other changes, so you can add task to project and user to project**
 *insert into project.data name = name, description = <the same syntax>, deadline = also the same*
 neccessary is only project name, others are optional,
 syntax for task see below
 *insert into project.data project = project, task = {task data part as shown higher without adding projject name once more},* also you can use the other syntax as below
 *insert into project.data task = {data with project name included}*
 For users here another syntax
 *insert into project.data project = project, user = {user emails and between all emails add ',' }*
 this will add user to your projects user list
 
## Select function
 **select from /DB file name/ where /data input/  or  select all from /DB file name/(this is the same for all DB, shows all)** 
*for example*
### In User DB 
 *select from user.data where name = name, surname = surname, age = 20(you can serach with age also like this age > 20, age < 30), email = somemail, 
 position = position, mobile = some mobile number* this will find for you and will print user info you look for.
 
### In Task DB 
  *select from task.data where project = project, title = title, deadline = deadline*
   this is possible type of select in task
### In Project DB
  in project DB you can search only with project name and/or deadline, and all as you want
  *select from project.data where name = name, deadline = deadline*

## Delete function
**delete from /DB file name/ where /the same datas which you can use for search as shown higher/**
### In User DB
*delete all from user.data* to delete all DB
*delete all from user.data where /some data/* to delete all found users
*delete from user.data where /some data/* to delete only found user

### In Task DB and in Project DB
*delete from task.data where* will found task and will remove if you will agree                                                                                                                                  
## Update function
**update in /DB file name/ /new data with the same syntax/ where /old data with the same syntax to find/**

### In User DB
**You can update all data about user**
### In Task DB
**You can update ony Title, description, deadline, and users**
### In Project DB
**You can only update project description and deadline**
                                                                                                                                  
                                                                                                                                  
*** Thank you for your attention, i hope you will like this DB ***
