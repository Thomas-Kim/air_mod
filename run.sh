execute="rosrun air_mod air_mod --training res/training_images/coffee_maker --detection res/coffee_detection/ --threshold $1"
echo $execute
echo "Running with a threshold of $1"
`$execute`
