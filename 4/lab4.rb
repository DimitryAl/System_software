require_relative "semaphore"
require 'thread'

def generateNumber(x)
    t = rand($semaphore_array.length)
    while (t == x) || ( (x != 0) && (t == 0) )
        t = rand($semaphore_array.length)
    end
    return t
end


class Interface 
    attr_accessor :name, 
                  :phone,
                  :number
                  #:confirm

    def Wait(x)
        sleep(x)
    end

    def MakeCall(x)    #сделать звонок
        #сделать так, чтобы никто не мог позвонить полуэкту что-то типо if n == 0 генерить заново
        n = generateNumber(x)
        res = $semaphore_array[n]
        time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
        puts time + "\t" + @name + ' is calling to ' + $names[n]
        if res.count == 0
            res.up!(1)
            @phone.up!
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
        #@confirm = false

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


class Grandmother < Interface
    attr_accessor :connections

    def initialize(name, phone, number)
        @name = name
        @phone = phone
        @number = number
        #@confirm = false

        while true
            while @phone.count != 0
                Wait(1)
            end
            res = MakeCall(@number)
            if res[0] == true
                puts @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                $semaphore_array[res[1]].down!(1)
                @phone.down!(1)

            else
                puts 'Call failed, ' + @name + ' waits'
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



girlfriend_number = 2   # кол-во девушек
granny_number = 2       # кол-во бабушек
n = 4                   # кол-во семафоров

$confirmation = [false]*n
$confirmation[0] = true
$names = []*n
$names[0] = 'Polyeuctus'
$names[1] = 'Mother'
$semaphore_array = []*n
for i in (1..n)
    $semaphore_array << Semaphore.new(1) 
end

$threads = []
$threads << Thread.new {Polyeuctus.new('Polyeuctus', $semaphore_array[0], 0)}
$threads << Thread.new {Mother.new('Mother', $semaphore_array[1], 1)}
# for i in (1..granny_number)
#     $names[i+1] = 'Granny' + i.to_s
#     $threads << Thread.new {granny = Grandmother.new('Granny' + i.to_s, $semaphore_array[i+1], i+1)}
# end

# РАБОТАЕТ
# $names[2] = 'Girlfriend_' + 1.to_s
# $threads << Thread.new {Girlfriend.new('Girlfriend_' + 1.to_s, $semaphore_array[2], 2)}
# $names[3] = 'Girlfriend_' + 2.to_s
# $threads << Thread.new {Girlfriend.new('Girlfriend_' + 2.to_s, $semaphore_array[3], 3)}

# ХЗ ПОЧЧЕМУ НЕ РАБОТАЕТ
for i in (2..3)
    $names[i] = 'Girlfriend_' + i.to_s
    $threads << Thread.new {Girlfriend.new('Girlfriend_' + i.to_s, $semaphore_array[i], i)}
end

# ХЗ ПОЧЕМУ НЕ РАБОТАЕТ
# i = 2
# while i < girlfriend_number + 2
#     $names[i] = 'Girlfriend_' + (i-1).to_s
#     $threads << Thread.new {Girlfriend.new('Girlfriend_' + (i-1).to_s, $semaphore_array[i], i)}
#     i = i + 1
# end 

$threads.each { |thread| thread.join}

# for i in (0..n-1)
#     puts
#     print $names[i]
#     print "\t"
#     print $semaphore_array[i].count
#     print "\t"
#     print $confirmation[i]
#     print "\t"
#     puts
# end

puts $threads