require_relative "semaphore"
require 'thread'

def generateNumber(x)
    t = rand($semaphore_array.length)
    while (t == x) || (t == 0)
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
        @phone.up!(1)
        n = generateNumber(x)
        res = $semaphore_array[n]
        time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
        puts time + "\t" + @name + ' is calling to ' + $names[n]
        if res.count == 0
            res.up!(1)
        #    @phone.up!
            if @name == 'Polyeuctus'        #тот, кому позвонил Полуэкт, получает подтверждение
                $confirmation[n] = true
            end
            if n == 0                       #если звонящий позвонил Полуэкту, то звонящий получает подтверждение
                $confirmation[x] = true
            end
            if $confirmation[n] == true
                $confirmation[x] = true
            end
            return true, n
        else
            @phone.down!(1)
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
                #Wait(1)
            end
            res = MakeCall(@number)   
            if res[0] == true
                #puts @name + ' successfully called to ' + $names[res[1]]
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(2)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                $semaphore_array[res[1]].down!(1)
                @phone.down!(1)
                break
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
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

        while true
            while @phone.count != 0
                #Wait(1)
            end
            if $confirmation[@number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' got confirmation'
                break
            end  
            # мб перед MakeCall сделать @phone.up!(1) у всех классов???
            res = MakeCall(@number)    
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                $semaphore_array[res[1]].down!(1)
                @phone.down!(1) 
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end
    end

end


class Grandmother < Interface
    attr_accessor :connections

    def initialize(name, phone, number, n)
        @name = name
        @phone = phone
        @number = number
        @connections = [false]*n
        @connections[0] = true
        @connections[@number] = true
        
        while true
            while @phone.count != 0
            end
            if !(@connections.include? false)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' got confirmation from all other people'
                break
            end
            res = MakeCall(@number)
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                @connections[res[1]] = true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                $semaphore_array[res[1]].down!(1)
                @phone.down!(1)
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end
    end

end


class Girlfriend < Interface
    
    def initialize(name, phone, number)
        @name = name
        @phone = phone
        @number = number
        
        while true  
            while @phone.count != 0
                #Wait(1)
            end
            if $confirmation[@number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' got confirmation'
                break
            end 
            res = MakeCall(@number)
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                $semaphore_array[res[1]].down!(1)
                @phone.down!(1)
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end
    end

end



girlfriend_number = 1   # кол-во девушек
granny_number = 2       # кол-во бабушек
n = 2 + girlfriend_number + granny_number                   # кол-во семафоров

$confirmation = [false]*n
$confirmation[0] = true
$names = []*n
$names[0] = 'Polyeuctus'
$names[1] = 'Mother'
$semaphore_array = []*n
for i in (1..n)
    $semaphore_array << Semaphore.new(1) 
end

threads = []
threads << Thread.new {Polyeuctus.new('Polyeuctus', $semaphore_array[0], 0)}
threads << Thread.new {Mother.new('Mother', $semaphore_array[1], 1)}

girlfriend_number.times do |i|
    $names[i+2] = 'Girlfriend_' + (i+1).to_s
    threads << Thread.new {Girlfriend.new('Girlfriend_' + (i+1).to_s, $semaphore_array[i+2], i+2)}
end

granny_number.times do |i|
    $names[i+2+girlfriend_number] = 'Granny_' + (i+1).to_s
    threads << Thread.new {Grandmother.new('Granny_' + (i+1).to_s, $semaphore_array[i+2+girlfriend_number], i+2+girlfriend_number, n)}
end

threads.each { |thread| thread.join}