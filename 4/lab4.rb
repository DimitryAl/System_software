require_relative "semaphore"
require 'thread'

def generateNumber(x)
    t = rand($semaphore_array.length)
    while t == x
        t = rand($semaphore_array.length)
    end
    return t
end

class Interface 
    attr_accessor :name, 
                  :phone,
                  :number

    def Wait(x)
        sleep(x)
    end

    def MakeCall(x)    #сделать звонок
        n = generateNumber(x)
        res = $semaphore_array[n]
        puts @name + ' is calling to ' + $names[n]
        if res.count == 0
            res.up!(1)
            @phone.up!
            return true, n
        else
            return false, -1
        end
    end

end

class Polyeuctus < Interface

    def initialize(name, phone, number)
        @name = name
        @phone = phone
        @number = number

        while true  #Пытается дозвониться до кого-то и если успешно, то перестает
            while @phone.count != 0
                Wait(1)
            end
            test = MakeCall(@number)   
            if test[0] == true
                puts @name + ' successfully called to ' + $names[test[1]]
                Wait(2)
                $semaphore_array[test[1]].down!(1)
                break
            else
                puts 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end    
    end

end

class Mother < Interface
    
    def initialize(name, phone, number)
        @name = name
        @phone = phone 
        @number = number

        #puts 'This is ' + @name

    end
    

end


class Grandmother < Interface
    #attr_accessor :arr[N], :mom=1, :gmom
    def initialize(name)
        @name = name
        puts "This's granny"
    end

    def Talk 
    end

end



class Girlfriend < Interface
    #attr_accessor :name 
    def initialize(name)
        @name = name
        puts "This is girlfriend"
    end

    def Talk 
    end

end


n = 2                   # кол-во семафоров
girlfriend_number = 2   # кол-во девушек
granny_number = 3       # кол-во бабушек

$names = []*n
$names[0] = 'Polyectus'
$names[1] = 'Mother'
$semaphore_array = []*n
for i in (1..n)
    $semaphore_array << Semaphore.new(1) 
end

$threads = []
$threads << Thread.new {pol = Polyeuctus.new('Polyeuctus', $semaphore_array[0], 0)}
$threads << Thread.new {mom = Mother.new('Mother', $semaphore_array[1], 1)}
# for i in (1..girlfriend_number)
#     threads << Thread.new {girl = Girlfriend.new('Girlfriend' + i.to_s)}
# end
# for i in (1..granny_number)
#     threads << Thread.new {granny = Grandmother.new('Granny' + i.to_s)}
# end




$threads.each { |thread| thread.join}