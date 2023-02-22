<div id="top"></div>

[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/miron-boiangiu/hogsmeade_train_station">
    <img src="images/logo.png" alt="Logo" width="120" height="120">
  </a>

<h3 align="center">Hogsmeade Train Station</h3>

  <p align="center">
    The first Data Structures and Algorithms homework we had, based on using linked lists to keep a record of trains and their cargo. 
    <br />
    <br />
    <a href="https://github.com/miron-boiangiu/hogsmeade_train_station/issues">Report Bug</a>
    ·
    <a href="https://github.com/miron-boiangiu/hogsmeade_train_station/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Coded in C, this project lays the groundwork for what could become a fully fledged application for managing trains for a train station. 
<br>
<a href="https://github.com/miron-boiangiu/hogsmeade_train_station/blob/main/Tema1.pdf">Assignment link.</a>

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [C](http://www.open-std.org/jtc1/sc22/wg14/)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started



### Prerequisites

* make & gcc
  ```sh
  sudo apt-get install build-essential
  ```

### Compilation

1. Simply use the makefile included in order to compile the program.
   ```sh
   make
   ```
2. You can then run the program by running ./run.sh.
   ```sh
   ./run.sh
   ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage
Here is a quick rundown of all the functions implemented:
<br>

<details>
  <summary>TrainStation* open_train_station(int platforms_no)</summary>

```
allocates memory for a new train station
with the desired number of platforms on it. It also sets all of the pointers to Trains
to NULL by using calloc.
```

</details>
<br>

<details>
  <summary>void close_train_station(TrainStation *station)</summary>

```
deallocates memory for an existing train station
by removing all trains from it first (using leave_train(station, platform)) and then removing
the station itself.
```

</details>
<br>

<details>
  <summary>void show_existing_trains(TrainStation *station, FILE *f)</summary>

```
goes through all of the platforms
of a station and checks if there is a train on them. If there is, it first prints the train then
all of its traincars.
```

</details>
<br>

<details>
  <summary>void arrive_train(TrainStation *station, int platform, int locomotive_power)</summary>

```
if possible, allocates
memory for a train and puts it on the station at the platform specified, then sets it locomotive power.
It also has a pointer to the list of traincars, which starts as NULL.
```

</details>
<br>

<details>
  <summary>void leave_train(TrainStation *station, int platform)</summary>

```
deallocates memory for all the traincars associated
to a train and then the memory of the train itself, and sets the platform as being free by setting it
to NULL. This function uses the auxiliary function void remove_all_traincars(TrainCar* traincar).
```

</details>
<br>

<details>
  <summary>void remove_all_traincars(TrainCar* traincar)</summary>

```
recursive function which puts on the top of the stack
the last traincars, then removes them one by one. The last one removed is the first traincar. It is done
like so in order to maintain a path to the following traincars, which would be lost if we started by
removing the first traincar.
```

</details>
<br>

<details>
  <summary>void add_train_car(TrainStation *station, int platform, int weight)</summary>

```
checks if it is possible to add
a traincar to the respective train, and, if it is, allocates memory for it and puts it at the end
of the list of traincars.
```

</details>
<br>

<details>
  <summary>void remove_train_cars(TrainStation *station, int platform, int weight)</summary>

```
if there is a train on
that specific platform, checks for all the traincars if they have the undesired weight and then removes
them, constantly updating the list of traincars. It starts by checking if the first traincars have
the undesired weight and only moves on to the rest of the traincars when it is done with all of the ones
that have that weight in the beginning, because they require an update to the train's pointer to the
traincars as well.
```

</details>
<br>

<details>
  <summary>int count_train_cars(Train* train)</summary>

```
counts the number of traincars that a train points to. Nothing
interesting to see here. Seriously, stop reading, move along. Dude, why are you still reading this?
Also, do you seriously read the READMEs? I didn't expect you to, damn. This was the most difficult
function to write so far, it took me several attempts over several days over several banged heads
against the wall.
```

</details>
<br>

<details>
  <summary>void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
int cars_no, int platform_b, int pos_b)</summary>

```
an absolutely disgusting piece of code that barely works.
First, it checks for a bunch of stuff that the function caller could get wrong and makes sure that
the move is actually possible. After wards, it takes a pointer to the traincars we need to move
and puts them in their respective place. It also rejoins the connection of the traincar before the ones
we removed to the one after the traincars we moved. This was, actually, the most difficult function
in all of this task.
```

</details>
<br>

<details>
  <summary>int traincar_sequence_sum(Train* train, int pos, int how_many)</summary>

```
another auxiliary function which
made my life so much easier and I'm glad I thought of it! It simply sums up <how_many> traincars
pointed to by the train, starting from the position <pos>. It is used in  all of the following functions,
by far the best investment of my life!
```

</details>
<br>

<details>
  <summary>int find_express_train(TrainStation *station)</summary>

```
using int traincar_sequence_sum() and count_train_cars(), 
it sums up the weight of the traincars of each train and checks if they have the biggest locomotive power
to traincars weight difference. It returns the platform number of the fastest train.
```
</details>
<br>

<details>
  <summary>int find_overload_train(TrainStation *station)</summary>

```
using int traincar_sequence_sum() and count_train_cars(), 
it sums up the weight of the traincars of each train and checks if the traincar can move. It returns the 
platform number of the overloaded train.
```
</details>
<br>

<details>
  <summary>int find_optimal_train(TrainStation *station)</summary>

```
calculates the difference between the train's power and its
traincars' weight for all of the trains on the platform. If they have the smallest difference so far, it 
remembers the platform the train was located at. At the end, it returns the train with the smallest difference
(that can still move).
```
</details>
<br>

<details>
  <summary>int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car)</summary>

```
this is where  
traincar_sequence_sum() really shines: the function goes through all of the trains and memorizes where
it found the heaviest sequence of <cars_no> traincars, in a similar fashion to the couple of functions before
this one.
```
</details>
<br>

<details>
  <summary>int positionOfHeaviestTrainCar(Train* train)</summary>

```
another auxiliary function I made! It goes
through the traincars of a train and returns the position of the heaviest one.
```
</details>
<br>

<details>
  <summary>void order_train(TrainStation *station, int platform)</summary>

```
it uses positionOfHeaviestTrainCar(Train* train) to check
which traincar is the heaviest one pointed to by a train and then moves it to a new sequence of traincars.
It does this for all of the traincars initially pointed to by the train, then makes the train point to the new
sequence of traincars.
```
</details>
<br>

<details>
  <summary>int sumOfAllTraincarsExceptOne(Train* train, int pos)</summary>

```
this is an auxiliary function that uses an auxiliary
function! Wow! It sums up the weight of all of the traincars EXCEPT the one at the specified position. 
```
</details>
<br>

<details>
  <summary>void fix_overload_train(TrainStation *station)</summary>

```
first, it uses find_overload_train() to see where we are going to
need to remove a traincar. Then, it goes through all of the traincars of it and uses sumOfAllTraincarsExceptOne()
to see if, by removing that one, the train can now move and if the weight moves is the largest one so far.
It then removes the traincar which made the train move the most weight.
```
</details>
<br>
<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Boiangiu Victor-Miron - miron.boiangiu@gmail.com

Project Link: [https://github.com/miron-boiangiu/hogsmeade_train_station](https://github.com/miron-boiangiu/hogsmeade_train_station)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/miron-boiangiu/hogsmeade_train_station.svg?style=for-the-badge
[contributors-url]:https://github.com/miron-boiangiu/hogsmeade_train_station/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/miron-boiangiu/hogsmeade_train_station.svg?style=for-the-badge
[forks-url]:https://github.com/miron-boiangiu/hogsmeade_train_station/network/members
[stars-shield]: https://img.shields.io/github/stars/miron-boiangiu/hogsmeade_train_station.svg?style=for-the-badge
[stars-url]:https://github.com/miron-boiangiu/hogsmeade_train_station/stargazers
[issues-shield]: https://img.shields.io/github/issues/miron-boiangiu/hogsmeade_train_station.svg?style=for-the-badge
[issues-url]:https://github.com/miron-boiangiu/hogsmeade_train_station/issues
[license-shield]: https://img.shields.io/github/license/miron-boiangiu/hogsmeade_train_station.svg?style=for-the-badge
[license-url]:https://github.com/miron-boiangiu/hogsmeade_train_station/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
