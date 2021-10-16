require_relative "semaphore"
require 'thread'

class Interface 
    attr_accessor :name, 
                  :phone

    def Wait()
        sleep(2)
    end

    def MakeCall    #сделать звонок
        # обращаемся к случайному элементу массива, если count == 0, то false,  иначе true        
        #n = rand($threads.length)
        n = 1
        test =  $threads[n]
        return true
        # if $threads[n].phone.count == 0
            
        # end

    end

end

class Polyeuctus < Interface

    def initialize(name, phone)
        @name = name
        @phone = phone
        puts 'This is ' + @name
                        
        while true  #Пытается дозвониться до кого-то и если успешно, то перестает
            puts @name + ' trying to call someone'
            if MakeCall()   
                puts @name + 'called to '
                sleep(2)
                break
            else
                puts name + ' sleeps for 2'
                Wait()
            end
        end    
    end

end

class Mother < Interface
    
    def initialize(name, phone)
        @name = name
        @phone = phone 
        
        puts 'This is ' + @name

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

semaphore_array = []
for i in (1..n)
    semaphore_array << Semaphore.new(1)
end

$threads = []
$threads << Thread.new {pol = Polyeuctus.new('Polyeuctus', semaphore_array[0])}
$threads << Thread.new {mom = Mother.new('Mother', semaphore_array[1])}
# for i in (1..girlfriend_number)
#     threads << Thread.new {girl = Girlfriend.new('Girlfriend' + i.to_s)}
# end
# for i in (1..granny_number)
#     threads << Thread.new {granny = Grandmother.new('Granny' + i.to_s)}
# end






$threads.each { |thread| thread.join}
