require 'monitor'
require 'thread'


def generateNumber(x)
    t = rand($phone_array.length)
    while (t == x) || (t == 0)
        t = rand($phone_array.length)
    end
    return t
end


class Interface 

    def Wait(x)
        sleep(x)
    end

    def MakeCall(x, name)    #сделать звонок
        n = generateNumber(x)
        time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
        puts time + "\t" + name + ' is calling to ' + $names[n]
        if $phone_array[n] == 0
            $phone_array[n] += 1
            if name == 'Polyeuctus'        #тот, кому позвонил Полуэкт, получает подтверждение
                $confirmation[n] = true
            end
            if $confirmation[n] == true
                $confirmation[x] = true
            end
            return true, n
        else
            $phone_array[x] -= 1
            return false, -1
        end
    end

end

class Caller < Interface
    include MonitorMixin
    
    def initialize(n)
        @arr = []
        for i in (1..n)
            @smth = new_cond 
            @arr << @smth   
        end
    end

    def polyeuctus(name, number)
        while true  
            
            mon_synchronize do
                if $phone_array[number] != 0
                    @arr[number].wait
                end
                $phone_array[number] += 1 
                
            end         
            res = MakeCall(number, name)   
            if res[0] == true
                
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' successfully called to ' + $names[res[1]]
                Wait(2)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' finished call'

                mon_synchronize do 
                    $phone_array[number] -= 1
                    $phone_array[res[1]] -= 1
                    @arr[res[1]].signal
                end
                break

            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + name + ' waits'
                Wait(2)
            end
        end    
    end


    def mother(name, number)
        while true 
            mon_synchronize do
                if $phone_array[number] != 0
                    @arr[number].wait
                end
                $phone_array[number] += 1
            end
            if $confirmation[number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' got confirmation'
                $phone_array[number] -= 1
                break
            end  
            res = MakeCall(number, name)  
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' finished call'
                

                mon_synchronize do 
                    $phone_array[number] -= 1
                    $phone_array[res[1]] -= 1
                    @arr[res[1]].signal
                end
                
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + name + ' waits'
                Wait(2)
            end
        end
    end

    def girlfriend(name, number)
        
        while true  
            self.mon_synchronize do
                if $phone_array[number] != 0
                    @arr[number].wait
                end
                $phone_array[number] += 1
           end
            if $confirmation[number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' got confirmation'
                $phone_array[number] -= 1
                break
            end 
            res = MakeCall(number, name)
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' finished call'
                mon_synchronize do 
                    $phone_array[number] -= 1
                    $phone_array[res[1]] -= 1
                    @arr[res[1]].signal
                end
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + name + ' waits'
                
                Wait(2)
            end
        end
    end

    def grandmother(name, number, n)
        
        connections = [false]*n
        connections[0] = true
        connections[number] = true

        while true
            mon_synchronize do
                if $phone_array[number] != 0
                    @arr[number].wait
                end
                $phone_array[number] += 1
            end

            if !(connections.include? false)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' got confirmation from all other people'
                $phone_array[number] -= 1
                break
            end
            res = MakeCall(number, name)
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                connections[res[1]] = true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + name + ' finished call'
                #$phone_array[res[1]] -= 1
                mon_synchronize do
                    $phone_array[number] -= 1
                    $phone_array[res[1]] -= 1
                    @arr[res[1]].signal
                end
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + name + ' waits'
                Wait(2)
            end
        end

    end

end



girlfriend_number = 1   # кол-во девушек
granny_number = 2       # кол-во бабушек
n = 2 + girlfriend_number + granny_number                   

$confirmation = [false]*n
$confirmation[0] = true
$names = []*n
$names[0] = 'Polyeuctus'
$names[1] = 'Mother'
$phone_array = [0]*n



threads = []
c = Caller.new(n)
threads << Thread.new {
    # p = Caller.new('Polyeuctus', 0)
    # p.polyeuctus
    c.polyeuctus('Polyeuctus', 0)
}
threads << Thread.new {
    # m = Caller.new('Mother', 1)
    # m.mother
    c.mother('Mother', 1)
}

girlfriend_number.times do |i|
     $names[i+2] = 'Girlfriend_' + (i+1).to_s
     threads << Thread.new {
        # g = Caller.new('Girlfriend_' + (i+1).to_s, i+2)
        # g.girlfriend
        c.girlfriend($names[i+2], i+2)
    }
end

granny_number.times do |i|
    $names[i+2+girlfriend_number] = 'Granny_' + (i+1).to_s
    threads << Thread.new {
        c.grandmother($names[i+2+girlfriend_number], i+2+girlfriend_number, n)
    }
end

threads.each { |thread| thread.join}