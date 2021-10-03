require_relative "semaphore"
require 'thread'

class Interface 
    attr_accessor :name,  
    :phone  #переменная, показывающая занят ли телефон

    def Wait
        sleep(10)
    end

end

class Polyeuctus < Interface
    #attr_accessor :name 
    def initialize()
        #@name = name
        puts "This is Polyeuctus"
        while true
            # if MakeCall() 
            #     break  #Пытается дозвониться до кого-то и если успешно, то перестает
            # end
            $test.down!(1)
            puts $my_variable
        end    
    end

    def MakeCall    #сделать звонок
        #if success then 
        return true 
        #else return false
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

class Mother < Interface
    #attr_accessor :name 
    def initialize(name)
        @name = name
        puts "This's mom"
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


N = 3
$my_variable = 10   #global variable
$test = Semaphore.new(N)

threads = []
threads << Thread.new {pol = Polyeuctus.new}
for i in (1..N)
    threads << Thread.new {granny = Grandmother.new(i)}
end

threads.each { |thread| thread.join}
