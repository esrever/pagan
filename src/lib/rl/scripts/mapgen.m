function M = mapgen()
    % TODO: corner perimeters connection adding: no!
   close all
   global conn_bit room_bit corr_bit perim_bit block_bit;
   block_bit = 1;
   conn_bit = 2;
   room_bit = 3;
   corr_bit = 4;
   perim_bit = 5;
   
   
   M = int32(zeros(180,180));
   M(1,:) = bitset(M(1,:),block_bit);
   M(end,:) = bitset(M(end,:),block_bit);
   M(:,1) = bitset(M(:,1),block_bit);
   M(:,end) = bitset(M(:,end),block_bit);
   
   bendiness = 0.2; %0.2;
   imagesc(M)
   hold on
   axis equal
   start_pt = floor(size(M)/2);
   M = create_room(M,start_pt, start_pt + [0,1],bendiness);
   M(21:-1:10,41);
   M(1:3,1:3);
   hold on, imagesc(M)
end

function drawme(M)
    hold on, imagesc(M);
    drawnow
end

function res = valid_idx(M, idx)
    r = size(M,1);
    c = size(M,2);
    res = (idx(1) > 0) && (idx(2) > 0) && (idx(1) <= r) && (idx(2) <= c);
end

function M = bitsetM(M,p,bit)
    M(p(1),p(2)) = bitset(M(p(1),p(2)), bit);
end

function res = bitgetM(M,p,bit)
    res = bitget(M(p(1),p(2)), bit);
end

function [next , rest] =select_next_tile(prev, cur, bendiness)
    dir = cur-prev;
    front = cur+dir;
    dir_tan = dir([2 1]);
    tans = [cur + dir_tan; cur - dir_tan];
    ridx = randi([1 2]);
    if rand > bendiness,
        next = front;
        rest = tans([ridx 3-ridx],:);
    else
        next = tans( ridx,:);
        rest = [front; tans(3-ridx,:)];
    end
end

function [M,conns] = carve_room(M, conn, rr, rc, bendiness)
    global conn_bit room_bit corr_bit perim_bit block_bit;
    rrp = [rr(1)-1 rr rr(end)+1];
    rcp = [rc(1)-1 rc rc(end)+1];
    M(rr,rc) = bitset(M(rr,rc), room_bit);
    
    % BEFORE we fix the perimeter, we calculate the new connections, as we
    % need the perimeter bits from other parts.
    % sub2ind( size(M), )
    
    old_perims = bitget(M, perim_bit);
    
    % perimeter fix
    M(rrp,rc(1)-1) = bitset(M(rrp,rc(1)-1), perim_bit);
    M(rrp,rc(end)+1) = bitset(M(rrp,rc(end)+1), perim_bit);
    M(rr(1)-1,rcp) = bitset(M(rr(1)-1,rcp), perim_bit);
    M(rr(end)+1,rcp) = bitset(M(rr(end)+1,rcp), perim_bit);
    % set connection
    M(conn(1),conn(2)) = 0;
    M(conn(1),conn(2)) = bitset( M(conn(1),conn(2)), conn_bit);
    
    new_perims = bitget(M, perim_bit);
    
    % get entries on the NEW perimeter only, that are NOT the prev conn
    [np_i,np_j] = find(bitxor(old_perims, new_perims));
    np_ij = [np_i np_j];
    %conn_1d = sub2ind(size(M),conn(1),conn(2));
    np_1d = sub2ind(size(M),np_ij(:,1),np_ij(:,2));
    
    rownum = size(M,1);
    elem = sub2ind(size(M),conn(1), conn(2));
    np_1d = np_1d( np_1d ~= (elem-1));
    np_1d = np_1d( np_1d ~= (elem+1));
    np_1d = np_1d( np_1d ~= (elem-rownum));
    np_1d = np_1d( np_1d ~= (elem+rownum));
    
    % also remove the 4 diagonals as candidates
    np_1d = np_1d( np_1d ~= sub2ind(size(M),rrp(1), rcp(1)));
    np_1d = np_1d( np_1d ~= sub2ind(size(M),rrp(1), rcp(end)));
    np_1d = np_1d( np_1d ~= sub2ind(size(M),rrp(end), rcp(1)));
    np_1d = np_1d( np_1d ~= sub2ind(size(M),rrp(end), rcp(end)));
    
    max_conns = 4;
    
    i=0;
    tries = 0;
    while true
        tries = tries+1;
        % if set is empty, break
        if isempty(np_1d) || (i >= max_conns) || (tries > (max_conns*2))
            break
        end
        % pick one at random
        idx = randi( numel(np_1d));
        elem = np_1d(idx);
        [elem_i, elem_j] = ind2sub(size(M), elem);
        if bitget( M(elem_i,elem_j), block_bit)
            continue;
        end
        
        % start a corridor 
        if true,
            hold on, imagesc(M)
            
            %M(elem_i,elem_j) = bitset( M(elem_i,elem_j), conn_bit);
            dirs = [1 0;-1 0;0 1;0 -1];
            for j=1:4,
               test_idx = [elem_i+dirs(j,1) elem_j+dirs(j,2)];
               if valid_idx(M,test_idx)
                   if M(test_idx(1),test_idx(2)) == 0,
                       [elem_i, elem_j];
                       M = create_corridor( M, [elem_i, elem_j], dirs(j,:), 25, bendiness);
                       break;
                   end
               end
            end
        end
        
        % eliminate neighbours as candidates
        rownum = size(M,1);
        np_1d = np_1d( np_1d ~= (elem-1));
        np_1d = np_1d( np_1d ~= (elem+1));
        np_1d = np_1d( np_1d ~= (elem-rownum));
        np_1d = np_1d( np_1d ~= (elem+rownum));
        
        i = i+1;
    end
    
    conns = [];
    
    conns = [conn conns];
end

function M = create_room( M, conn, corr, bendiness)
    drawme(M)
    global conn_bit room_bit corr_bit perim_bit;
    mcols = size(M,2);
    mrows = size(M,1);
    max_tries = 100;
    for itry = 1:max_tries,
        % create a random room size
        cols = randi([4 10]);
        rows = randi([4 10]);
        dir = conn - corr;
        gen_range_const_side = @(x,w,step) (x+step):step:(x+w);
        gen_range_main_side = @(x,w,r,step) (x-r+1):step:(x+(w-r));
        
        if isequal(dir, [1 0]) % bottom
            range_rows = gen_range_const_side( conn(1),rows,1);
            range_cols = gen_range_main_side( conn(2),cols, randi(cols),1);
        elseif isequal(dir, [-1 0])
            range_rows = gen_range_const_side( conn(1),-rows,-1);
            range_cols = gen_range_main_side( conn(2),cols, randi(cols),1);
        elseif isequal(dir, [0 1])
            range_cols = gen_range_const_side( conn(2),cols,1);
            range_rows = gen_range_main_side( conn(1),rows, randi(rows),1);
        elseif isequal(dir, [0 -1])
            range_cols = gen_range_const_side( conn(2),-cols,-1);
            range_rows = gen_range_main_side( conn(1),rows, randi(rows),1);
        else
            assert(false)
        end
        range_cols = sort(range_cols);
        range_rows = sort(range_rows);
        if (range_cols(1) <= 0) |(range_rows(1) <= 0) | (range_cols(end) > mcols) | (range_rows(end) > mrows)
           continue 
        end
        Msub = M(range_rows, range_cols);
        % if all are unused
        if  ~(any(any(Msub))),
           M = carve_room(M, conn, range_rows, range_cols,bendiness);
           M = create_corridor(M,conn,corr-conn,25,bendiness);
           break
        end
    end
end

function M = create_corridor( M, start, dir, avg_len, bendiness)
    clf
    drawme(M)
    global conn_bit room_bit corr_bit perim_bit block_bit;
    % calculate the end length & start digging
    end_len = floor( 2 + rand * floor(1.5*avg_len));
    len = 1;
    prev_tile = start-dir;
    cur_tile = start;
    oldold_dir = cur_tile - prev_tile;
    make_room_at_end = true;
    % select the next tile
    [new_tile, rest] = select_next_tile(prev_tile,cur_tile,0);
    if bitgetM(M,new_tile, room_bit)
        return
    end
    while len < end_len,
        %hold on, imagesc(M)
        Mval = M(new_tile(1),new_tile(2));
        % if it's blocked, change direction
        new_dir = new_tile - cur_tile;
        uturn = isequal(new_dir,-oldold_dir);
        if (bitget( Mval, block_bit) | uturn ),
            found = false;
            for i=1:2,
                new_tile = rest(i,:);
                Mval = M(new_tile(1),new_tile(2));
                if ~bitget( Mval, block_bit)
                    found = true;
                    break;
                end
            end
            if ~found,
                make_room_at_end = false;
                break;
            end
        end
        % if it's perimeter or connection, add connection
        if bitget( Mval, perim_bit) || bitget( Mval, conn_bit),
           %assert(false) % Triggers after the dir change
           % TODO: if it's NOT diagonal, add connection
           if true,
               
               % look around new tile, in 4 directions
               % try to find at least one room/corr
               dirs = [1 0;-1 0;0 1;0 -1];
               found = false;
               for i=1:4,
                  nbtile = new_tile + dirs(i,:);
                  if ~isequal(nbtile, cur_tile),
                      if bitgetM(M, nbtile, corr_bit) || bitgetM(M, nbtile, room_bit)
                          found = true;
                          break;
                      end
                  end
               end
               
               if found,
                   M(new_tile(1),new_tile(2)) = 0;
                   M(new_tile(1),new_tile(2)) = bitset( M(new_tile(1),new_tile(2)), conn_bit);
               end
           end
           
           max_ij = cur_tile+1;
           min_ij = cur_tile-1;
           % I want the 2x3 that doesn't include the new_tile!
           if new_tile(1) == cur_tile(1),
               if new_tile(2) > cur_tile(2),
                   max_ij(2) = max_ij(2)-1;
               else
                   min_ij(2) = min_ij(2)+1;
               end
           else
               if new_tile(1) > cur_tile(1),
                   max_ij(1) = max_ij(1)-1;
               else
                   min_ij(1) = min_ij(1)+1;
               end
           end

           % Set perimeter bit
           % (new_tile - cur_tile) ~= 0: find which axis has diff
           % new_tile * diff
           for i=min_ij(1):max_ij(1),
               for j=min_ij(2):max_ij(2),
                   is_corr = bitget(M(i,j),corr_bit);
                   is_conn = bitget(M(i,j),conn_bit);
                   is_room = bitget(M(i,j),room_bit);
                   if ~(is_corr | is_conn | is_room)
                       M(i,j) = bitset(M(i,j), perim_bit);
                   end
               end
           end 
           break;
        else
           M(new_tile(1),new_tile(2)) = bitshift(1,corr_bit-1);
           % all the surrounding that are not corr OR conn, bitset them to perim
           %max_ij = max( new_tile, cur_tile);
           %min_ij = min( new_tile, cur_tile);
           [cur_tile;new_tile];
           max_ij = cur_tile+1;
           min_ij = cur_tile-1;
           % I want the 2x3 that doesn't include the new_tile!
           if new_tile(1) == cur_tile(1),
               if new_tile(2) > cur_tile(2),
                   max_ij(2) = max_ij(2)-1;
               else
                   min_ij(2) = min_ij(2)+1;
               end
           else
               if new_tile(1) > cur_tile(1),
                   max_ij(1) = max_ij(1)-1;
               else
                   min_ij(1) = min_ij(1)+1;
               end
           end
           
           % (new_tile - cur_tile) ~= 0: find which axis has diff
           % new_tile * diff
           for i=min_ij(1):max_ij(1),
               for j=min_ij(2):max_ij(2),
                   is_corr = bitget(M(i,j),corr_bit);
                   is_conn = bitget(M(i,j),conn_bit);
                   is_room = bitget(M(i,j),room_bit);
                   if ~(is_corr | is_conn | is_room)
                       M(i,j) = bitset(M(i,j), perim_bit);
                   end
               end
           end
           drawme(M)
           len = len+ 1;
           % TODO: add a connection to one of the tangent tiles if length
           % has been more than 5 + 2d6
           if (len > (5 + sum(randi([1 6],[1 2])))) && false,
              tandir = new_tile - cur_tile;
              tandir = tandir([2 1]);
              nbs = [cur_tile + tandir;cur_tile - tandir;];
              nbpt = nbs(randi([1 2]),:);
              if ~bitgetM(M,nbpt,block_bit)
                  if randi([1 2]) == 1,
                     M2 = create_room( M, cur_tile, prev_tile,bendiness);
                     if ~isequal(M2,M)
                        M = M2;
                        M(nbpt(1),nbpt(2)) = 0;
                        M = bitsetM(M,nbpt,conn_bit);
                     else
                         % TODO: add perimeter
                         display('dead-end')
                         min_ij = cur_tile-1;
                         max_ij = cur_tile+1;
                         for i=min_ij(1):max_ij(1),
                             for j=min_ij(2):max_ij(2),
                                 is_corr = bitget(M(i,j),corr_bit);
                                 is_conn = bitget(M(i,j),conn_bit);
                                 is_room = bitget(M(i,j),room_bit);
                                 if ~(is_corr | is_conn | is_room)
                                     M(i,j) = bitset(M(i,j), perim_bit);
                                 end
                             end
                         end
                     end
                  else
                     curdir = nbpt - cur_tile;
                     M = create_corridor( M, nbpt, curdir, 25, bendiness);
                  end
              end
           end
        end
        
        % rotate tiles
        oldold_dir = cur_tile - prev_tile;
        prev_tile = cur_tile;
        cur_tile = new_tile;
        % select the next tile
        [new_tile, rest] = select_next_tile(prev_tile,cur_tile,bendiness);
    end
    % make the start a connection only if the len is at least 2
    if len > 1,
       M(start(1),start(2)) = 0;
       M(start(1),start(2)) = bitset(M(start(1),start(2)), conn_bit); 
    end
    
    M2 = M;
    if make_room_at_end,
       M2 = create_room( M, cur_tile, prev_tile,bendiness); 
    end
    if ~isequal(M2,M)
        M = M2;
    else
       % TODO: add perimeter 
       display('dead-end')
       min_ij = cur_tile-1;
       max_ij = cur_tile+1;
       for i=min_ij(1):max_ij(1),
           for j=min_ij(2):max_ij(2),
               is_corr = bitget(M(i,j),corr_bit);
               is_conn = bitget(M(i,j),conn_bit);
               is_room = bitget(M(i,j),room_bit);
               if ~(is_corr | is_conn | is_room)
                   M(i,j) = bitset(M(i,j), perim_bit);
               end
           end
       end
    end
end