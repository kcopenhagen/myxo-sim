function makeAllVideo()
%%
    fpath = uigetdir('/Users/kc32/Desktop/mnt/localtigress/myxoSim');    

    files = dir(fullfile(fpath,"frames/"));
    files2 = dir(fullfile(fpath,"water/"));
    del = [];

    for i = 1:numel(files)
        if (files(i).name(1) == '.') || ~contains(files(i).name,'0')
            del = [del; i];
        end
    end
    files(del) = [];
    files2(del) = [];

    lines = readlines(fullfile(fpath,'parameters.h'));

    bSstr = lines(contains(lines,'boxSize'));
    BS = regexp(bSstr,'\d+\.?\d*','match');
    boxSize = str2double(BS{1});

    wR = lines(contains(lines,'waterRadius'));
    WR = regexp(wR,'\d+\.?\d*','match');
    waterRadius = str2double(WR{1});

    wrF = lines(contains(lines,'waterRadiusFactor'));
    WRF = regexp(wrF,'\d+\.?\d*','match');
    waterRadiusFactor = str2double(WRF{1});

    cR = lines(contains(lines,'beadRadius'));
    CR = regexp(cR,'\d+\.?\d*','match');
    beadRadius = str2double(CR);

    v = VideoWriter(fullfile(fpath,'AllVideo.mp4'),'MPEG-4');
    v.Quality = 95;
    v.FrameRate = 30;
    open(v);

    fig = figure('Units','pixels','Position',[0 0 400 400],'Color','w');
    ax = axes(fig,'Position',[0 0 1 1]);
    set(ax,'Visible','off','xlim',[0 boxSize],'ylim',[0 boxSize],'clim', [-0.3 0.1]);
    hold(ax,'on');
    col = [];

    x = [];
    y = [];
    z = [];
    c = [];
    a = [];

    s = surface([x';x'],[y';y'],[z';z'],[c';c'],...
            'facecol','no',...
            'edgecol','flat',...
            'linew',1.5);
    s.AlphaData = [a';a'];
    s.AlphaDataMapping = 'none';
    s.EdgeAlpha = 'flat';
    hold(ax,'on');
    p = plot3(x,y,z,'.');
    
    for t = 1:numel(files)
        cells = readmatrix(fullfile(files(t).folder,files(t).name));
        water = readmatrix(fullfile(files2(t).folder,files2(t).name));
        
        xs = cells(:,1);
        x2s = water(:,1);
        ys = cells(:,2);
        y2s = water(:,2);
        zs = cells(:,3);
        z2s = water(:,3) - waterRadiusFactor * waterRadius;

        ids = round(cells(:,4))+1;
        uids = unique(ids);
        while numel(col)<max(uids)
            col = [col; rand()];
        end
        x = [];
        y = [];
        z = [];
        a = [];
        c = [];
        for i = 1:numel(uids)
            cids = ids == uids(i);
            xt = xs(cids);
            yt = ys(cids);
            zt = zs(cids);
            x = [x; xt];
            y = [y; yt];
            z = [z; zt];
            c = [c; col(uids(i))*ones(sum(cids),1)];
            
            dxs = xt(1:end-1)-xt(2:end);
            dys = yt(1:end-1)-yt(2:end);
            drs = sqrt(dxs.^2+dys.^2);
            a = [a; drs<boxSize/2];
            a = [a; 0];

        end
        c = z;
        s.XData = [x';x'];
        s.YData = [y';y'];
        s.ZData = [z';z'];
        s.CData = [c';c'];
        s.AlphaData = [a';a'];
        s.AlphaDataMapping = 'none';
        s.EdgeAlpha = 'flat';
        if isempty(p)
            p = scatter3(x2s,y2s,z2s,20,z2s,'filled');
        end
        p.XData = x2s;
        p.YData = y2s;
        p.ZData = z2s;
        p.CData = z2s;
        F = getframe;
        writeVideo(v, F.cdata);
    end
    close(v);
end