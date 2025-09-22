# If-else statements
x = 10

if x > 5:
    print("x is greater than 5")
else:
    print("x is less than or equal to 5")

# While loop
count = 0
while count < 5:
    print("Count:", count)
    count = count + 1

# Nested conditions
grade = 85

if grade >= 90:
    print("Grade A")
else:
    if grade >= 80:
        print("Grade B")
    else:
        if grade >= 70:
            print("Grade C")
        else:
            print("Grade F")